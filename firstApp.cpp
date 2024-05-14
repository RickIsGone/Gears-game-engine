#include "firstApp.hpp"

#include <stdexcept>

namespace engine{

    FirstApp::FirstApp(){
        createPipelineLayout();
        createPipeline();
        createCommandBuffer();
    }

    FirstApp::~FirstApp(){
        vkDestroyPipelineLayout(engineDevice.device(), pipelineLayout, nullptr);
    }

    void FirstApp::run(){
        while(!engineWindow.shouldClose()){
            glfwPollEvents();
        }

        engineWindow.~EngineWindow();           // da tenere per ora 
    }
    
    void FirstApp::createPipelineLayout(){
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;
        if(vkCreatePipelineLayout(engineDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) throw std::runtime_error("failed to create a pipeline layout");
    }

    void FirstApp::createPipeline(){
        PipelineConfigInfo pipelineConfig{};
        EnginePipeline::defaultPipelineConfigInfo(pipelineConfig, engineSwapChain.width(), engineSwapChain.height());
        pipelineConfig.renderPass = engineSwapChain.getRenderPass();                                                                                                
        pipelineConfig.pipelineLayout = pipelineLayout;
        enginePipeline = std::make_unique<EnginePipeline>(engineDevice, "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv", pipelineConfig);   // fa crashare programma 
    }   
    

    void FirstApp::createCommandBuffer(){
        
    }

    void FirstApp::drawFrame(){
        
    }
}