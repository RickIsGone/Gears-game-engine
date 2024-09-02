#include <array>

#include "engineRenderer.hpp"
#include "logger.hpp"

namespace gears {

   EngineRenderer::EngineRenderer(EngineWindow& window, EngineDevice& device) : engineWindow{window}, engineDevice{device} {
      recreateSwapChain();
      createCommandBuffers();
   }

   EngineRenderer::~EngineRenderer() { freeCommandBuffers(); }

   void EngineRenderer::recreateSwapChain() {
      auto extent = engineWindow.getExtent();
      while (extent.width == 0 || extent.height == 0) {
         extent = engineWindow.getExtent();
         glfwWaitEvents();
      }

      vkDeviceWaitIdle(engineDevice.device());

      if (engineSwapChain == nullptr) engineSwapChain = std::make_unique<EngineSwapChain>(engineDevice, extent);
      else {
         std::shared_ptr<EngineSwapChain> oldSwapChain = std::move(engineSwapChain);
         engineSwapChain = std::make_unique<EngineSwapChain>(engineDevice, extent, oldSwapChain);
         if (!oldSwapChain->compareSwapFormats(*engineSwapChain.get())) throw Logger::loggerException("swapChain image format has changed");
      }

      // se renderpass compatibile non fare nulla
      // createPipeline();
   }

   void EngineRenderer::createCommandBuffers() {
      commandBuffers.resize(EngineSwapChain::MAX_FRAMES_IN_FLIGHT);

      VkCommandBufferAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
      allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
      allocInfo.commandPool = engineDevice.getCommandPool();
      allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

      if (vkAllocateCommandBuffers(engineDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) throw Logger::loggerException("failed to allocate command buffers");
   }

   void EngineRenderer::freeCommandBuffers() {
      vkFreeCommandBuffers(engineDevice.device(), engineDevice.getCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
      commandBuffers.clear();
   }

   VkCommandBuffer EngineRenderer::beginFrame() {
      assert(!isFrameStarted && "can't call beginFrame while already in progress");

      auto result = engineSwapChain->acquireNextImage(&currentImageIndex);

      if (result == VK_ERROR_OUT_OF_DATE_KHR) {
         recreateSwapChain();
         return nullptr;
      }

      if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) throw Logger::loggerException("failed to acquire next swap chain image");

      isFrameStarted = true;

      auto commandBuffer = getCurrentCommandBuffer();

      VkCommandBufferBeginInfo beginInfo{};
      beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

      if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) throw Logger::loggerException("failed to begin recording command buffer");

      return commandBuffer;
   }

   void EngineRenderer::endFrame() {
      assert(isFrameStarted && "can't call endFrame while frame not in progress");

      auto commandBuffer = getCurrentCommandBuffer();

      if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) throw Logger::loggerException("failed to record command buffer");

      auto result = engineSwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
      if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || engineWindow.wasWindowResized()) {
         engineWindow.resetWindowResizeFlag();
         recreateSwapChain();
      } else if (result != VK_SUCCESS)
         throw Logger::loggerException("failed to present swap chain image");

      isFrameStarted = false;
      currentFrameIndex = (currentFrameIndex + 1) % EngineSwapChain::MAX_FRAMES_IN_FLIGHT;
   }

   void EngineRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer) {
      assert(isFrameStarted && "can't call beginFrame while frame not in progress");
      assert(commandBuffer == getCurrentCommandBuffer() && "can't begin renderPass on command buffer from diffrent frame");

      VkCommandBufferBeginInfo beginInfo{};
      beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

      VkRenderPassBeginInfo renderPassInfo{};
      renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
      renderPassInfo.renderPass = engineSwapChain->getRenderPass();
      renderPassInfo.framebuffer = engineSwapChain->getFrameBuffer(currentImageIndex);

      renderPassInfo.renderArea.offset = {0, 0};
      renderPassInfo.renderArea.extent = engineSwapChain->getSwapChainExtent();

      std::array<VkClearValue, 2> clearValues{};
      clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
      clearValues[1].depthStencil = {1.0f, 0};

      renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
      renderPassInfo.pClearValues = clearValues.data();

      vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

      VkViewport viewport{};
      viewport.x = 0.0f;
      viewport.y = 0.0f;
      viewport.width = static_cast<float>(engineSwapChain->getSwapChainExtent().width);
      viewport.height = static_cast<float>(engineSwapChain->getSwapChainExtent().height);
      viewport.minDepth = 0.0f;
      viewport.maxDepth = 1.0f;
      VkRect2D scissor{{0, 0}, engineSwapChain->getSwapChainExtent()};
      vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
      vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
   }

   void EngineRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer) {
      assert(isFrameStarted && "can't call endSwapChainRenderPass while frame not in progress");
      assert(commandBuffer == getCurrentCommandBuffer() && "can't end renderPass on command buffer from diffrent frame");

      vkCmdEndRenderPass(commandBuffer);
   }

} // namespace gears
