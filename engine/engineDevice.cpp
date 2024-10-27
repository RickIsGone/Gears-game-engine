#include <cstring>
#include <set>
#include <unordered_set>

#include "engineDevice.hpp"
#include "logger.hpp"

namespace gears {

   // local callback functions
   static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
       VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
       VkDebugUtilsMessageTypeFlagsEXT messageType,
       const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
       void* pUserData) {
      logger->warn("validation layer: " + (std::string)pCallbackData->pMessage);

      return VK_FALSE;
   }

   VkResult CreateDebugUtilsMessengerEXT(
       VkInstance _instance,
       const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
       const VkAllocationCallbacks* pAllocator,
       VkDebugUtilsMessengerEXT* pDebugMessenger) {
      auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
          _instance,
          "vkCreateDebugUtilsMessengerEXT");
      if (func != nullptr) {
         return func(_instance, pCreateInfo, pAllocator, pDebugMessenger);
      } else {
         return VK_ERROR_EXTENSION_NOT_PRESENT;
      }
   }

   void DestroyDebugUtilsMessengerEXT(
       VkInstance _instance,
       VkDebugUtilsMessengerEXT _debugMessenger,
       const VkAllocationCallbacks* pAllocator) {
      auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
          _instance,
          "vkDestroyDebugUtilsMessengerEXT");
      if (func != nullptr) {
         func(_instance, _debugMessenger, pAllocator);
      }
   }

   // class member functions
   PhysicalDevice::PhysicalDevice(Window& window) : _window{window} {
      _createInstance();
      _setupDebugMessenger();
      _createSurface();
      _pickPhysicalDevice();
      _createLogicalDevice();
      _createCommandPool();
   }

   PhysicalDevice::~PhysicalDevice() {
      vkDestroyCommandPool(_device, _commandPool, nullptr);
      vkDestroyDevice(_device, nullptr);

      if (enableValidationLayers) {
         DestroyDebugUtilsMessengerEXT(_instance, _debugMessenger, nullptr);
      }

      vkDestroySurfaceKHR(_instance, _surface, nullptr);
      vkDestroyInstance(_instance, nullptr);
   }

   void PhysicalDevice::_createInstance() {
      if (enableValidationLayers && !_checkValidationLayerSupport()) {
         throw Logger::Exception("validation layers requested, but not available!");
      }

      VkApplicationInfo appInfo = {};
      appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
      appInfo.pApplicationName = "Gears game engine";
      appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
      appInfo.pEngineName = "Gears";
      appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
      appInfo.apiVersion = VK_API_VERSION_1_0;

      VkInstanceCreateInfo createInfo = {};
      createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
      createInfo.pApplicationInfo = &appInfo;

      auto extensions = _getRequiredExtensions();
      createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
      createInfo.ppEnabledExtensionNames = extensions.data();

      VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
      if (enableValidationLayers) {
         createInfo.enabledLayerCount = static_cast<uint32_t>(_validationLayers.size());
         createInfo.ppEnabledLayerNames = _validationLayers.data();

         _populateDebugMessengerCreateInfo(debugCreateInfo);
         createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
      } else {
         createInfo.enabledLayerCount = 0;
         createInfo.pNext = nullptr;
      }

      if (vkCreateInstance(&createInfo, nullptr, &_instance) != VK_SUCCESS) {
         throw Logger::Exception("failed to create _instance!");
      }

      _hasGflwRequiredInstanceExtensions();
   }

   void PhysicalDevice::_pickPhysicalDevice() {
      uint32_t deviceCount = 0;
      vkEnumeratePhysicalDevices(_instance, &deviceCount, nullptr);
      if (deviceCount == 0) {
         throw Logger::Exception("failed to find GPUs with Vulkan support!");
      }
      logger->log("Device count: " + std::to_string(deviceCount));
      std::vector<VkPhysicalDevice> devices(deviceCount);
      vkEnumeratePhysicalDevices(_instance, &deviceCount, devices.data());

      for (const auto& device : devices) {
         if (_isDeviceSuitable(device)) {
            _physicalDevice = device;
            break;
         }
      }

      if (_physicalDevice == VK_NULL_HANDLE) {
         throw Logger::Exception("failed to find a suitable GPU!");
      }

      vkGetPhysicalDeviceProperties(_physicalDevice, &properties);
      logger->log("physical device: " + (std::string)properties.deviceName);
   }

   void PhysicalDevice::_createLogicalDevice() {
      QueueFamilyIndices indices = _findQueueFamilies(_physicalDevice);

      std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
      std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily, indices.presentFamily};

      float queuePriority = 1.0f;
      for (uint32_t queueFamily : uniqueQueueFamilies) {
         VkDeviceQueueCreateInfo queueCreateInfo = {};
         queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
         queueCreateInfo.queueFamilyIndex = queueFamily;
         queueCreateInfo.queueCount = 1;
         queueCreateInfo.pQueuePriorities = &queuePriority;
         queueCreateInfos.push_back(queueCreateInfo);
      }

      VkPhysicalDeviceFeatures deviceFeatures = {};
      deviceFeatures.samplerAnisotropy = VK_TRUE;

      VkDeviceCreateInfo createInfo = {};
      createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

      createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
      createInfo.pQueueCreateInfos = queueCreateInfos.data();

      createInfo.pEnabledFeatures = &deviceFeatures;
      createInfo.enabledExtensionCount = static_cast<uint32_t>(_deviceExtensions.size());
      createInfo.ppEnabledExtensionNames = _deviceExtensions.data();

      // might not really be necessary anymore because device specific validation layers
      // have been deprecated
      if (enableValidationLayers) {
         createInfo.enabledLayerCount = static_cast<uint32_t>(_validationLayers.size());
         createInfo.ppEnabledLayerNames = _validationLayers.data();
      } else {
         createInfo.enabledLayerCount = 0;
      }

      if (vkCreateDevice(_physicalDevice, &createInfo, nullptr, &_device) != VK_SUCCESS) {
         throw Logger::Exception("failed to create logical device!");
      }

      vkGetDeviceQueue(_device, indices.graphicsFamily, 0, &_graphicsQueue);
      vkGetDeviceQueue(_device, indices.presentFamily, 0, &_presentQueue);
   }

   void PhysicalDevice::_createCommandPool() {
      QueueFamilyIndices queueFamilyIndices = findPhysicalQueueFamilies();

      VkCommandPoolCreateInfo poolInfo = {};
      poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
      poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
      poolInfo.flags =
          VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

      if (vkCreateCommandPool(_device, &poolInfo, nullptr, &_commandPool) != VK_SUCCESS) {
         throw Logger::Exception("failed to create command pool!");
      }
   }

   void PhysicalDevice::_createSurface() { _window.createWindowSurface(_instance, &_surface); }

   bool PhysicalDevice::_isDeviceSuitable(VkPhysicalDevice device) {
      QueueFamilyIndices indices = _findQueueFamilies(device);

      bool extensionsSupported = _checkDeviceExtensionSupport(device);

      bool swapChainAdequate = false;
      if (extensionsSupported) {
         SwapChainSupportDetails swapChainSupport = _querySwapChainSupport(device);
         swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
      }

      VkPhysicalDeviceFeatures supportedFeatures;
      vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

      return indices.isComplete() && extensionsSupported && swapChainAdequate &&
          supportedFeatures.samplerAnisotropy;
   }

   void PhysicalDevice::_populateDebugMessengerCreateInfo(
       VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
      createInfo = {};
      createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
      createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
          VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
      createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
          VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
          VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
      createInfo.pfnUserCallback = debugCallback;
      createInfo.pUserData = nullptr; // Optional
   }

   void PhysicalDevice::_setupDebugMessenger() {
      if (!enableValidationLayers) return;
      VkDebugUtilsMessengerCreateInfoEXT createInfo;
      _populateDebugMessengerCreateInfo(createInfo);
      if (CreateDebugUtilsMessengerEXT(_instance, &createInfo, nullptr, &_debugMessenger) != VK_SUCCESS) {
         throw Logger::Exception("failed to set up debug messenger!");
      }
   }

   bool PhysicalDevice::_checkValidationLayerSupport() {
      uint32_t layerCount;
      vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

      std::vector<VkLayerProperties> availableLayers(layerCount);
      vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

      for (const char* layerName : _validationLayers) {
         bool layerFound = false;

         for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
               layerFound = true;
               break;
            }
         }

         if (!layerFound) {
            return false;
         }
      }

      return true;
   }

   std::vector<const char*> PhysicalDevice::_getRequiredExtensions() {
      uint32_t glfwExtensionCount = 0;
      const char** glfwExtensions;
      glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

      std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

      if (enableValidationLayers) {
         extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
      }

      return extensions;
   }

   void PhysicalDevice::_hasGflwRequiredInstanceExtensions() {
      uint32_t extensionCount = 0;
      vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
      std::vector<VkExtensionProperties> extensions(extensionCount);
      vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

      logger->log("available extensions:");
      std::unordered_set<std::string> available;
      for (const auto& extension : extensions) {
         logger->logNoTrace("\t" + (std::string)extension.extensionName);
         available.insert(extension.extensionName);
      }

      logger->log("required extensions:");
      auto requiredExtensions = _getRequiredExtensions();
      for (const auto& required : requiredExtensions) {
         logger->logNoTrace("\t" + (std::string)required);
         if (available.find(required) == available.end()) {
            throw Logger::Exception("Missing required glfw extension");
         }
      }
   }

   bool PhysicalDevice::_checkDeviceExtensionSupport(VkPhysicalDevice device) {
      uint32_t extensionCount;
      vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

      std::vector<VkExtensionProperties> availableExtensions(extensionCount);
      vkEnumerateDeviceExtensionProperties(
          device,
          nullptr,
          &extensionCount,
          availableExtensions.data());

      std::set<std::string> requiredExtensions(_deviceExtensions.begin(), _deviceExtensions.end());

      for (const auto& extension : availableExtensions) {
         requiredExtensions.erase(extension.extensionName);
      }

      return requiredExtensions.empty();
   }

   QueueFamilyIndices PhysicalDevice::_findQueueFamilies(VkPhysicalDevice device) {
      QueueFamilyIndices indices;

      uint32_t queueFamilyCount = 0;
      vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

      std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
      vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

      int i = 0;
      for (const auto& queueFamily : queueFamilies) {
         if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
            indices.graphicsFamilyHasValue = true;
         }
         VkBool32 presentSupport = false;
         vkGetPhysicalDeviceSurfaceSupportKHR(device, i, _surface, &presentSupport);
         if (queueFamily.queueCount > 0 && presentSupport) {
            indices.presentFamily = i;
            indices.presentFamilyHasValue = true;
         }
         if (indices.isComplete()) {
            break;
         }

         i++;
      }

      return indices;
   }

   SwapChainSupportDetails PhysicalDevice::_querySwapChainSupport(VkPhysicalDevice device) {
      SwapChainSupportDetails details;
      vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, _surface, &details.capabilities);

      uint32_t formatCount;
      vkGetPhysicalDeviceSurfaceFormatsKHR(device, _surface, &formatCount, nullptr);

      if (formatCount != 0) {
         details.formats.resize(formatCount);
         vkGetPhysicalDeviceSurfaceFormatsKHR(device, _surface, &formatCount, details.formats.data());
      }

      uint32_t presentModeCount;
      vkGetPhysicalDeviceSurfacePresentModesKHR(device, _surface, &presentModeCount, nullptr);

      if (presentModeCount != 0) {
         details.presentModes.resize(presentModeCount);
         vkGetPhysicalDeviceSurfacePresentModesKHR(
             device,
             _surface,
             &presentModeCount,
             details.presentModes.data());
      }
      return details;
   }

   VkFormat PhysicalDevice::findSupportedFormat(
       const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
      for (VkFormat format : candidates) {
         VkFormatProperties props;
         vkGetPhysicalDeviceFormatProperties(_physicalDevice, format, &props);

         if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
            return format;
         } else if (
             tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
            return format;
         }
      }
      throw Logger::Exception("failed to find supported format!");
   }

   uint32_t PhysicalDevice::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
      VkPhysicalDeviceMemoryProperties memProperties;
      vkGetPhysicalDeviceMemoryProperties(_physicalDevice, &memProperties);
      for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
         if ((typeFilter & (1 << i)) &&
             (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
         }
      }

      throw Logger::Exception("failed to find suitable memory type!");
   }

   void PhysicalDevice::createBuffer(
       VkDeviceSize size,
       VkBufferUsageFlags usage,
       VkMemoryPropertyFlags properties,
       VkBuffer& buffer,
       VkDeviceMemory& bufferMemory) {
      VkBufferCreateInfo bufferInfo{};
      bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
      bufferInfo.size = size;
      bufferInfo.usage = usage;
      bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

      if (vkCreateBuffer(_device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
         throw Logger::Exception("failed to create vertex buffer!");
      }

      VkMemoryRequirements memRequirements;
      vkGetBufferMemoryRequirements(_device, buffer, &memRequirements);

      VkMemoryAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
      allocInfo.allocationSize = memRequirements.size;
      allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

      if (vkAllocateMemory(_device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
         throw Logger::Exception("failed to allocate vertex buffer memory!");
      }

      vkBindBufferMemory(_device, buffer, bufferMemory, 0);
   }

   VkCommandBuffer PhysicalDevice::beginSingleTimeCommands() {
      VkCommandBufferAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
      allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
      allocInfo.commandPool = _commandPool;
      allocInfo.commandBufferCount = 1;

      VkCommandBuffer commandBuffer;
      vkAllocateCommandBuffers(_device, &allocInfo, &commandBuffer);

      VkCommandBufferBeginInfo beginInfo{};
      beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
      beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

      vkBeginCommandBuffer(commandBuffer, &beginInfo);
      return commandBuffer;
   }

   void PhysicalDevice::endSingleTimeCommands(VkCommandBuffer commandBuffer) {
      vkEndCommandBuffer(commandBuffer);

      VkSubmitInfo submitInfo{};
      submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
      submitInfo.commandBufferCount = 1;
      submitInfo.pCommandBuffers = &commandBuffer;

      vkQueueSubmit(_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
      vkQueueWaitIdle(_graphicsQueue);

      vkFreeCommandBuffers(_device, _commandPool, 1, &commandBuffer);
   }

   void PhysicalDevice::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
      VkCommandBuffer commandBuffer = beginSingleTimeCommands();

      VkBufferCopy copyRegion{};
      copyRegion.srcOffset = 0; // Optional
      copyRegion.dstOffset = 0; // Optional
      copyRegion.size = size;
      vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

      endSingleTimeCommands(commandBuffer);
   }

   void PhysicalDevice::copyBufferToImage(
       VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount) {
      VkCommandBuffer commandBuffer = beginSingleTimeCommands();

      VkBufferImageCopy region{};
      region.bufferOffset = 0;
      region.bufferRowLength = 0;
      region.bufferImageHeight = 0;

      region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      region.imageSubresource.mipLevel = 0;
      region.imageSubresource.baseArrayLayer = 0;
      region.imageSubresource.layerCount = layerCount;

      region.imageOffset = {0, 0, 0};
      region.imageExtent = {width, height, 1};

      vkCmdCopyBufferToImage(
          commandBuffer,
          buffer,
          image,
          VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
          1,
          &region);
      endSingleTimeCommands(commandBuffer);
   }

   void PhysicalDevice::createImageWithInfo(
       const VkImageCreateInfo& imageInfo,
       VkMemoryPropertyFlags properties,
       VkImage& image,
       VkDeviceMemory& imageMemory) {
      if (vkCreateImage(_device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
         throw Logger::Exception("failed to create image!");
      }

      VkMemoryRequirements memRequirements;
      vkGetImageMemoryRequirements(_device, image, &memRequirements);

      VkMemoryAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
      allocInfo.allocationSize = memRequirements.size;
      allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

      if (vkAllocateMemory(_device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
         throw Logger::Exception("failed to allocate image memory!");
      }

      if (vkBindImageMemory(_device, image, imageMemory, 0) != VK_SUCCESS) {
         throw Logger::Exception("failed to bind image memory!");
      }
   }

} // namespace gears