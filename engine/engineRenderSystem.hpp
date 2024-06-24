#pragma once

#include <memory>
#include <vector>

#include "enginePipeline.hpp"
#include "engineDevice.hpp"
#include "engineGameObject.hpp"
#include "engineCamera.hpp"

namespace gears {

   class EngineRenderSystem {
   public:
      EngineRenderSystem(EngineDevice &device, VkRenderPass renderPass);
      ~EngineRenderSystem();

      EngineRenderSystem(const EngineRenderSystem &) = delete;
      EngineRenderSystem &operator=(const EngineRenderSystem &) = delete;

      void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<EngineGameObject> &gameObjects, const EngineCamera &camera);

   private:
      void createPipelineLayout();
      void createPipeline(VkRenderPass renderPass);

      EngineDevice &engineDevice;
      std::unique_ptr<EnginePipeline> enginePipeline;
      VkPipelineLayout pipelineLayout;
   };
} // namespace gears