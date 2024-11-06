module;

#include <memory>
#include <vector>
#include <cassert>
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <array>

export module engineRenderer;
import logger;
import engineWindow;
import engineDevice;
import engineSwapChain;

namespace gears {

   export class Renderer {
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

   //  ========================================== implementation ==========================================

   Renderer::Renderer(Window& window, PhysicalDevice& device) : _window{window}, _device{device} {
      _recreateSwapChain();
      _createCommandBuffers();
   }

   Renderer::~Renderer() { _freeCommandBuffers(); }

   void Renderer::_recreateSwapChain() {
      auto extent = _window.getExtent();
      while (extent.width == 0 || extent.height == 0) {
         extent = _window.getExtent();
         glfwWaitEvents();
      }

      vkDeviceWaitIdle(_device.device());

      if (_engineSwapChain == nullptr) _engineSwapChain = std::make_unique<EngineSwapChain>(_device, extent);
      else {
         std::shared_ptr<EngineSwapChain> oldSwapChain = std::move(_engineSwapChain);
         _engineSwapChain = std::make_unique<EngineSwapChain>(_device, extent, oldSwapChain);
         if (!oldSwapChain->compareSwapFormats(*_engineSwapChain.get())) throw Logger::Exception("swapChain image format has changed");
      }

      // se renderpass compatibile non fare nulla
      // createPipeline();
   }

   void Renderer::_createCommandBuffers() {
      _commandBuffers.resize(EngineSwapChain::MAX_FRAMES_IN_FLIGHT);

      VkCommandBufferAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
      allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
      allocInfo.commandPool = _device.getCommandPool();
      allocInfo.commandBufferCount = static_cast<uint32_t>(_commandBuffers.size());

      if (vkAllocateCommandBuffers(_device.device(), &allocInfo, _commandBuffers.data()) != VK_SUCCESS) throw Logger::Exception("failed to allocate command buffers");
   }

   void Renderer::_freeCommandBuffers() {
      vkFreeCommandBuffers(_device.device(), _device.getCommandPool(), static_cast<uint32_t>(_commandBuffers.size()), _commandBuffers.data());
      _commandBuffers.clear();
   }

   VkCommandBuffer Renderer::beginFrame() {
      assert(!_isFrameStarted && "can't call beginFrame while already in progress");

      auto result = _engineSwapChain->acquireNextImage(&_currentImageIndex);

      if (result == VK_ERROR_OUT_OF_DATE_KHR) {
         _recreateSwapChain();
         return nullptr;
      }

      if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) throw Logger::Exception("failed to acquire next swap chain image");

      _isFrameStarted = true;

      auto commandBuffer = getCurrentCommandBuffer();

      VkCommandBufferBeginInfo beginInfo{};
      beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

      if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) throw Logger::Exception("failed to begin recording command buffer");

      return commandBuffer;
   }

   void Renderer::endFrame() {
      assert(_isFrameStarted && "can't call endFrame while frame not in progress");

      auto commandBuffer = getCurrentCommandBuffer();

      if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) throw Logger::Exception("failed to record command buffer");

      auto result = _engineSwapChain->submitCommandBuffers(&commandBuffer, &_currentImageIndex);
      if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || _window.wasWindowResized()) {
         _window.resetWindowResizeFlag();
         _recreateSwapChain();
      } else if (result != VK_SUCCESS)
         throw Logger::Exception("failed to present swap chain image");

      _isFrameStarted = false;
      _currentFrameIndex = (_currentFrameIndex + 1) % EngineSwapChain::MAX_FRAMES_IN_FLIGHT;
   }

   void Renderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer) {
      assert(_isFrameStarted && "can't call beginFrame while frame not in progress");
      assert(commandBuffer == getCurrentCommandBuffer() && "can't begin renderPass on command buffer from diffrent frame");

      VkCommandBufferBeginInfo beginInfo{};
      beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

      VkRenderPassBeginInfo renderPassInfo{};
      renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
      renderPassInfo.renderPass = _engineSwapChain->getRenderPass();
      renderPassInfo.framebuffer = _engineSwapChain->getFrameBuffer(_currentImageIndex);

      renderPassInfo.renderArea.offset = {0, 0};
      renderPassInfo.renderArea.extent = _engineSwapChain->getSwapChainExtent();

      std::array<VkClearValue, 2> clearValues{};
      clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
      clearValues[1].depthStencil = {1.0f, 0};

      renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
      renderPassInfo.pClearValues = clearValues.data();

      vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

      VkViewport viewport{};
      viewport.x = 0.0f;
      viewport.y = 0.0f;
      viewport.width = static_cast<float>(_engineSwapChain->getSwapChainExtent().width);
      viewport.height = static_cast<float>(_engineSwapChain->getSwapChainExtent().height);
      viewport.minDepth = 0.0f;
      viewport.maxDepth = 1.0f;
      VkRect2D scissor{{0, 0}, _engineSwapChain->getSwapChainExtent()};
      vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
      vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
   }

   void Renderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer) {
      assert(_isFrameStarted && "can't call endSwapChainRenderPass while frame not in progress");
      assert(commandBuffer == getCurrentCommandBuffer() && "can't end renderPass on command buffer from diffrent frame");

      vkCmdEndRenderPass(commandBuffer);
   }

   void Renderer::enableVSync() {
      _engineSwapChain->enableVSync();
      _recreateSwapChain();
   }

   void Renderer::disableVSync() {
      _engineSwapChain->disableVSync();
      _recreateSwapChain();
   }

} // namespace gears