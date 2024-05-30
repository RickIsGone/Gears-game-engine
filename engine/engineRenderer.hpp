#pragma once

#include "engineWindow.hpp"
#include "engineDevice.hpp"
#include "engineSwapChain.hpp"

#include <memory>
#include <vector>
#include <cassert>

namespace gears{

    class EngineRenderer{
    public:
        EngineRenderer(EngineWindow &engineWindow, EngineDevice &engineDevice);
        ~EngineRenderer();

        EngineRenderer(const EngineRenderer &)= delete;
        EngineRenderer &operator=(const EngineRenderer &)= delete;

        bool isFrameInProgress() const{ return isFrameStarted; }
        
        float getAspectRatio() const{ return engineSwapChain->extentAspectRatio(); }

        VkRenderPass getSwapChainRenderPass() const{ return engineSwapChain->getRenderPass(); }
        VkCommandBuffer getCurrentCommandBuffer() const{ 
            assert(isFrameStarted && "cannot get command buffer when frame not in progress");
            return commandBuffers[currentFrameIndex];
        }

        int getFrameIndex() const{
            assert(isFrameStarted && "cannot get frame index when frame not in progress");
            return currentFrameIndex;
        }

        VkCommandBuffer beginFrame();
        void endFrame();
        void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
        void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

    private:

        void createCommandBuffers();
        void freeCommandBuffers();
        void recreateSwapChain();

        EngineWindow &engineWindow;
        EngineDevice &engineDevice;
        std::unique_ptr<EngineSwapChain> engineSwapChain; 
        std::vector<VkCommandBuffer> commandBuffers;

        uint32_t currentImageIndex;
        int currentFrameIndex{0};
        bool isFrameStarted{false};
    };
}