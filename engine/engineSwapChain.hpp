#pragma once

#include <vector>
#include <memory>

#include "engineDevice.hpp"

#include <vulkan/vulkan.hpp>


namespace gears {

   class EngineSwapChain {
   public:
      static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

      EngineSwapChain(PhysicalDevice& deviceRef, VkExtent2D extent, bool vSync = true);
      EngineSwapChain(PhysicalDevice& deviceRef, VkExtent2D extent, std::shared_ptr<EngineSwapChain> previous);
      ~EngineSwapChain();

      EngineSwapChain(const EngineSwapChain&) = delete;
      EngineSwapChain& operator=(const EngineSwapChain&) = delete;

      VkFramebuffer getFrameBuffer(int index) { return _swapChainFramebuffers[index]; }
      VkRenderPass getRenderPass() { return _renderPass; }
      VkImageView getImageView(int index) { return _swapChainImageViews[index]; }
      size_t imageCount() { return _swapChainImages.size(); }
      VkFormat getSwapChainImageFormat() { return _swapChainImageFormat; }
      VkExtent2D getSwapChainExtent() { return _swapChainExtent; }
      uint32_t width() { return _swapChainExtent.width; }
      uint32_t height() { return _swapChainExtent.height; }

      float extentAspectRatio() {
         return static_cast<float>(_swapChainExtent.width) / static_cast<float>(_swapChainExtent.height);
      }
      VkFormat findDepthFormat();

      VkResult acquireNextImage(uint32_t* imageIndex);
      VkResult submitCommandBuffers(const VkCommandBuffer* buffers, uint32_t* imageIndex);

      bool compareSwapFormats(const EngineSwapChain& swapChain) const { return swapChain._swapChainDepthFormat == _swapChainDepthFormat && swapChain._swapChainImageFormat == _swapChainImageFormat; }

      void enableVSync() { _vSync = true; }
      void disableVSync() { _vSync = false; }
      bool isVSyncEnabled() { return _vSync; }

   private:
      void _init();
      void _createSwapChain();
      void _createImageViews();
      void _createDepthResources();
      void _createRenderPass();
      void _createFramebuffers();
      void _createSyncObjects();

      // Helper functions
      VkSurfaceFormatKHR _chooseSwapSurfaceFormat(
          const std::vector<VkSurfaceFormatKHR>& availableFormats);
      VkPresentModeKHR _chooseSwapPresentMode(
          const std::vector<VkPresentModeKHR>& availablePresentModes);
      VkExtent2D _chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

      bool _vSync{true};

      VkFormat _swapChainImageFormat;
      VkFormat _swapChainDepthFormat;
      VkExtent2D _swapChainExtent;

      std::vector<VkFramebuffer> _swapChainFramebuffers;
      VkRenderPass _renderPass;

      std::vector<VkImage> _depthImages;
      std::vector<VkDeviceMemory> _depthImageMemorys;
      std::vector<VkImageView> _depthImageViews;
      std::vector<VkImage> _swapChainImages;
      std::vector<VkImageView> _swapChainImageViews;

      PhysicalDevice& _device;
      VkExtent2D _windowExtent;

      VkSwapchainKHR _swapChain;
      std::shared_ptr<EngineSwapChain> _oldSwapChain;

      std::vector<VkSemaphore> _imageAvailableSemaphores;
      std::vector<VkSemaphore> _renderFinishedSemaphores;
      std::vector<VkFence> _inFlightFences;
      std::vector<VkFence> _imagesInFlight;
      size_t _currentFrame = 0;
   };

} // namespace gears
