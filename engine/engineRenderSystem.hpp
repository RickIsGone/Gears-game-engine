#pragma once

#include "enginePipeline.hpp"
#include "engineDevice.hpp"
#include "engineGameObject.hpp"

#include <memory>
#include <vector>

namespace gears{

    class EngineRenderSystem{
    public:

        EngineRenderSystem(EngineDevice& device, VkRenderPass renderPass);
        ~EngineRenderSystem();

        EngineRenderSystem(const EngineRenderSystem &)= delete;
        EngineRenderSystem &operator=(const EngineRenderSystem &)= delete;

        void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<EngineGameObject>& gameObjects);

    private:

        void createPipelineLayout();
        void createPipeline(VkRenderPass renderPass);

        EngineDevice &engineDevice;
        std::unique_ptr<EnginePipeline> enginePipeline;
        VkPipelineLayout pipelineLayout;

    };
}