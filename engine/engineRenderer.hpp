#pragma once

#include <memory>
#include <vector>
#include <cassert>

#include "engineWindow.hpp"
#include "engineDevice.hpp"
#include "engineSwapChain.hpp"

namespace gears {

   class Renderer {
   public:
      Renderer(Window& _window, PhysicalDevice& _device);
      ~Renderer();

      Renderer(const Renderer&) = delete;
      Renderer& operator=(const Renderer&) = delete;

      bool isFrameInProgress() const { return _isFrameStarted; }

      float getAspectRatio() const { return _engineSwapChain->extentAspectRatio(); }

      VkRenderPass getSwapChainRenderPass() const { return _engineSwapChain->getRenderPass(); }
      VkCommandBuffer getCurrentCommandBuffer() const {
         assert(_isFrameStarted && "cannot get command buffer when frame not in progress");
         return _commandBuffers[_currentFrameIndex];
      }

      int getFrameIndex() const {
         assert(_isFrameStarted && "cannot get frame index when frame not in progress");
         return _currentFrameIndex;
      }

      VkCommandBuffer beginFrame();
      void endFrame();
      void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
      void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

      void enableVSync();
      void disableVSync();

   private:
      void _createCommandBuffers();
      void _freeCommandBuffers();
      void _recreateSwapChain();

      Window& _window;
      PhysicalDevice& _device;
      std::unique_ptr<EngineSwapChain> _engineSwapChain;
      std::vector<VkCommandBuffer> _commandBuffers;

      uint32_t _currentImageIndex;
      int _currentFrameIndex{0};
      bool _isFrameStarted{false};
   };
} // namespace gears