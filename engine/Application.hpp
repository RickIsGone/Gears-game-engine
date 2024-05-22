#pragma once

#include "engineWindow.hpp"
#include "enginePipeline.hpp"
#include "engineDevice.hpp"
#include "engineSwapChain.hpp"
#include "engineModel.hpp"

#include <memory>
#include <vector>

namespace gears{

    class Application{
    public:
        static constexpr int WIDTH = 1080;
        static constexpr int HEIGHT = 720;

        Application();
        ~Application();

        Application(const Application &)= delete;
        Application &operator=(const Application &)= delete;

        void run();
    private:

        void loadModels();
        void createPipelineLayout();
        void createPipeline();
        void createCommandBuffers();
        void freeCommandBuffers();
        void drawFrame();
        void recreateSwapChain();
        void recordCommandBuffer(int imageIndex);

        EngineWindow engineWindow{WIDTH, HEIGHT, "Gears engine goes brrrrrrrrrrrr"};
        EngineDevice engineDevice{engineWindow};
        std::unique_ptr<EngineSwapChain> engineSwapChain; 
        std::unique_ptr<EnginePipeline> enginePipeline;
        VkPipelineLayout pipelineLayout;
        std::vector<VkCommandBuffer> commandBuffers;
        std::unique_ptr<EngineModel> engineModel;

    };
}