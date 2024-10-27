#include "engineRenderSystem.hpp"
#include "logger.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace gears {
   struct SimplePushConstantData {
      glm::mat4 transform{1.0f};
      alignas(16) glm::vec3 color;
   };

   EngineRenderSystem::EngineRenderSystem(PhysicalDevice& device, VkRenderPass renderPass) : _device{device} {
      _createPipelineLayout();
      _createPipeline(renderPass);
   }

   EngineRenderSystem::~EngineRenderSystem() {
      vkDestroyPipelineLayout(_device.device(), _pipelineLayout, nullptr);
   }

   void EngineRenderSystem::_createPipelineLayout() {
      VkPushConstantRange pushConstantRange{};
      pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
      pushConstantRange.offset = 0;
      pushConstantRange.size = sizeof(SimplePushConstantData);

      VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
      pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
      pipelineLayoutInfo.setLayoutCount = 0;
      pipelineLayoutInfo.pSetLayouts = nullptr;
      pipelineLayoutInfo.pushConstantRangeCount = 1;
      pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
      if (vkCreatePipelineLayout(_device.device(), &pipelineLayoutInfo, nullptr, &_pipelineLayout) != VK_SUCCESS) throw Logger::Exception("failed to create a pipeline layout");
   }

   void EngineRenderSystem::_createPipeline(VkRenderPass renderPass) {
      assert(_pipelineLayout != nullptr && "cannot create pipeline before pipeline layout");

      PipelineConfigInfo pipelineConfig{};
      EnginePipeline::defaultPipelineConfigInfo(pipelineConfig);
      pipelineConfig.renderPass = renderPass;
      pipelineConfig.pipelineLayout = _pipelineLayout;
      _enginePipeline = std::make_unique<EnginePipeline>(_device, "shaders/shader.vert.spv", "shaders/shader.frag.spv", pipelineConfig);
   }

   void EngineRenderSystem::renderGameObjects(VkCommandBuffer commandBuffer, std::vector<EngineGameObject>& gameObjects, const EngineCamera& camera) {
      _enginePipeline->bind(commandBuffer);

      auto projectionView = camera.getProjection() * camera.getView();

      for (auto& obj : gameObjects) {
         // obj.transform.rotation.y = glm::mod<float>(obj.transform.rotation.y + 0.01f, 2.0f * glm::pi<float>());
         // obj.transform.rotation.x = glm::mod<float>(obj.transform.rotation.y + 0.001f, 2.0f * glm::pi<float>());

         SimplePushConstantData push{};
         push.color = obj.color;
         push.transform = projectionView * obj.transform.mat4();

         vkCmdPushConstants(
             commandBuffer,
             _pipelineLayout,
             VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
             0,
             sizeof(SimplePushConstantData),
             &push);
         obj.model->bind(commandBuffer);
         obj.model->draw(commandBuffer);
      }
   }
} // namespace gears
