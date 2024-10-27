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
      EngineRenderSystem(PhysicalDevice& device, VkRenderPass renderPass);
      ~EngineRenderSystem();

      EngineRenderSystem(const EngineRenderSystem&) = delete;
      EngineRenderSystem& operator=(const EngineRenderSystem&) = delete;

      void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<EngineGameObject>& gameObjects, const EngineCamera& camera);

   private:
      void _createPipelineLayout();
      void _createPipeline(VkRenderPass renderPass);

      PhysicalDevice& _device;
      std::unique_ptr<EnginePipeline> _enginePipeline;
      VkPipelineLayout _pipelineLayout;
   };
} // namespace gears