#pragma once

#include "engineWindow.hpp"
#include "enginePipeline.hpp"
#include "engineDevice.hpp"

namespace engine{

    class FirstApp{
    public:
        static constexpr int WIDTH = 1920;
        static constexpr int HEIGHT = 1080;

        void run();
    private:
        EngineWindow engineWindow{WIDTH, HEIGHT, "Engine goes brrrrrrrrrrrr"};
        EngineDevice engineDevice{engineWindow};
        EnginePipeline enginePipeline{engineDevice, "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv", EnginePipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)};
    };
}