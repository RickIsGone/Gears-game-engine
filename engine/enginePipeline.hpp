#pragma once

#include <string>
#include <vector>

#include "engineDevice.hpp"

namespace gears {

   struct PipelineConfigInfo {
      PipelineConfigInfo() = default;
      PipelineConfigInfo(const PipelineConfigInfo&) = delete;
      PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

      VkPipelineViewportStateCreateInfo viewportInfo;
      VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
      VkPipelineRasterizationStateCreateInfo rasterizationInfo;
      VkPipelineMultisampleStateCreateInfo multisampleInfo;
      VkPipelineColorBlendAttachmentState colorBlendAttachment;
      VkPipelineColorBlendStateCreateInfo colorBlendInfo;
      VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
      std::vector<VkDynamicState> dynamicStateEnables;
      VkPipelineDynamicStateCreateInfo dynamicStateInfo;
      VkPipelineLayout pipelineLayout = nullptr;
      VkRenderPass renderPass = nullptr;
      uint32_t subpass = 0;
   };

   class EnginePipeline {
   public:
      EnginePipeline(PhysicalDevice& device, const std::string& vertFilePath, const std::string& fragFilePath, const PipelineConfigInfo& configInfo);
      ~EnginePipeline();

      EnginePipeline(const EnginePipeline&) = delete;
      EnginePipeline& operator=(const EnginePipeline&) = delete;

      void bind(VkCommandBuffer VkCommandBuffer);

      static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);

   private:
      static std::vector<char> readFile(const std::string& filePath);
      void createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath, const PipelineConfigInfo& configInfo);

      void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

      PhysicalDevice& _device;
      VkPipeline _graphicsPipeline;
      VkShaderModule _vertShaderModule;
      VkShaderModule _fragShaderModule;
   };
} // namespace gears