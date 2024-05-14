#pragma once

#include "engineWindow.hpp"
#include "enginePipeline.hpp"
#include "engineDevice.hpp"
#include "engineSwapChain.hpp"

#include <memory>
#include <vector>

namespace engine{

    class FirstApp{
    public:
        static constexpr int WIDTH = 1080;
        static constexpr int HEIGHT = 720;

        FirstApp();
        ~FirstApp();

        FirstApp(const FirstApp &)= delete;
        FirstApp &operator=(const FirstApp &)= delete;

        void run();
    private:

        void createPipelineLayout();
        void createPipeline();
        void createCommandBuffers();
        void drawFrame();

        EngineWindow engineWindow{WIDTH, HEIGHT, "Engine goes brrrrrrrrrrrr"};
        EngineDevice engineDevice{engineWindow};
        EngineSwapChain engineSwapChain{engineDevice, engineWindow.getExtent()};
        std::unique_ptr<EnginePipeline> enginePipeline;
        VkPipelineLayout pipelineLayout;
        std::vector<VkCommandBuffer> commandBuffers;

    };
}