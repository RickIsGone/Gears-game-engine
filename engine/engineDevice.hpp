#pragma once

#include <vector>

#include "engineWindow.hpp"

namespace gears {

   struct SwapChainSupportDetails {
      VkSurfaceCapabilitiesKHR capabilities;
      std::vector<VkSurfaceFormatKHR> formats;
      std::vector<VkPresentModeKHR> presentModes;
   };

   struct QueueFamilyIndices {
      uint32_t graphicsFamily;
      uint32_t presentFamily;
      bool graphicsFamilyHasValue = false;
      bool presentFamilyHasValue = false;
      bool isComplete() { return graphicsFamilyHasValue && presentFamilyHasValue; }
   };

   class PhysicalDevice {
   public:
#ifdef GRS_DEBUG
      const bool enableValidationLayers = false;
#else
      const bool enableValidationLayers = true;
#endif // GRS_DEBUG

      PhysicalDevice(Window& window);
      ~PhysicalDevice();

      // Not copyable or movable
      PhysicalDevice(const PhysicalDevice&) = delete;
      PhysicalDevice& operator=(const PhysicalDevice&) = delete;
      PhysicalDevice(PhysicalDevice&&) = delete;
      PhysicalDevice& operator=(PhysicalDevice&&) = delete;

      VkCommandPool getCommandPool() { return _commandPool; }
      VkDevice device() { return _device; }
      VkSurfaceKHR surface() { return _surface; }
      VkQueue graphicsQueue() { return _graphicsQueue; }
      VkQueue presentQueue() { return _presentQueue; }

      SwapChainSupportDetails getSwapChainSupport() { return _querySwapChainSupport(_physicalDevice); }
      uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
      QueueFamilyIndices findPhysicalQueueFamilies() { return _findQueueFamilies(_physicalDevice); }
      VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

      // Buffer Helper Functions
      void createBuffer(
          VkDeviceSize size,
          VkBufferUsageFlags usage,
          VkMemoryPropertyFlags properties,
          VkBuffer& buffer,
          VkDeviceMemory& bufferMemory);
      VkCommandBuffer beginSingleTimeCommands();
      void endSingleTimeCommands(VkCommandBuffer commandBuffer);
      void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
      void copyBufferToImage(
          VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount);

      void createImageWithInfo(
          const VkImageCreateInfo& imageInfo,
          VkMemoryPropertyFlags properties,
          VkImage& image,
          VkDeviceMemory& imageMemory);

      VkPhysicalDeviceProperties properties;

   private:
      void _createInstance();
      void _setupDebugMessenger();
      void _createSurface();
      void _pickPhysicalDevice();
      void _createLogicalDevice();
      void _createCommandPool();

      // helper functions
      bool _isDeviceSuitable(VkPhysicalDevice device);
      std::vector<const char*> _getRequiredExtensions();
      bool _checkValidationLayerSupport();
      QueueFamilyIndices _findQueueFamilies(VkPhysicalDevice device);
      void _populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
      void _hasGflwRequiredInstanceExtensions();
      bool _checkDeviceExtensionSupport(VkPhysicalDevice device);
      SwapChainSupportDetails _querySwapChainSupport(VkPhysicalDevice device);

      VkInstance _instance;
      VkDebugUtilsMessengerEXT _debugMessenger;
      VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
      Window& _window;
      VkCommandPool _commandPool;

      VkDevice _device;
      VkSurfaceKHR _surface;
      VkQueue _graphicsQueue;
      VkQueue _presentQueue;

      const std::vector<const char*> _validationLayers = {"VK_LAYER_KHRONOS_validation"};
      const std::vector<const char*> _deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
   };

} // namespace gears