#pragma once

#include "engineDevice.hpp"


#include <string>
#include <vector>

namespace engine{

    struct PipelineConfigInfo {
        PipelineConfigInfo() = default;
        PipelineConfigInfo(const PipelineConfigInfo&) = delete;
        PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

        VkViewport viewport;
        VkRect2D scissor;
        VkPipelineViewportStateCreateInfo viewportInfo;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo colorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
        VkPipelineLayout pipelineLayout = nullptr;
        VkRenderPass renderPass = nullptr;
        uint32_t subpass = 0;
    };

    class EnginePipeline{
    public:
        EnginePipeline(EngineDevice &device, const std::string &vertFilePath, const std::string &fragFilePath, const PipelineConfigInfo& configInfo);  
        ~EnginePipeline();

        EnginePipeline(const EnginePipeline&) = delete;
        void operator=(const EnginePipeline&) = delete;

        void bind(VkCommandBuffer VkCommandBuffer);

        static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo, uint32_t width, uint32_t height);

    private:
        static std::vector<char> readFile(const std::string& filePath);
        void createGraphicsPipeline(const std::string &vertFilePath, const std::string &fragFilePath, const PipelineConfigInfo& configInfo);

        void createShaderModule(const std::vector<char> &code, VkShaderModule *shaderModule);

        EngineDevice &engineDevice;
        VkPipeline graphicsPipeline;
        VkShaderModule vertShaderModule;
        VkShaderModule fragShaderModule;
    };
}