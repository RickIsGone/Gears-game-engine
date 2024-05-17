#include "firstApp.hpp"

#include <stdexcept>
#include <array>

namespace engine{

    FirstApp::FirstApp(){
        loadModels();
        createPipelineLayout();
        createPipeline();
        createCommandBuffers();
    }

    FirstApp::~FirstApp(){
        vkDestroyPipelineLayout(engineDevice.device(), pipelineLayout, nullptr);
    }

    void FirstApp::run(){
        while(!engineWindow.shouldClose()){
            glfwPollEvents();
            drawFrame();
        }

        vkDeviceWaitIdle(engineDevice.device());

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
    

    void FirstApp::createCommandBuffers(){
        commandBuffers.resize(engineSwapChain.imageCount());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = engineDevice.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());
        
        if(vkAllocateCommandBuffers(engineDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) throw std::runtime_error("failed to allocate command buffers");

        for(int i = 0; i<commandBuffers.size(); ++i){
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            if(vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) throw std::runtime_error("failed to begin recording");

            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = engineSwapChain.getRenderPass();
            renderPassInfo.framebuffer = engineSwapChain.getFrameBuffer(i);

            renderPassInfo.renderArea.offset = {0,0};
            renderPassInfo.renderArea.extent = engineSwapChain.getSwapChainExtent();

            std::array<VkClearValue, 2> clearValues{};
            clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
            clearValues[1].depthStencil = {1.0f, 0};

            renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            enginePipeline->bind(commandBuffers[i]);
            engineModel->bind(commandBuffers[i]);
            engineModel->draw(commandBuffers[i]);

            vkCmdEndRenderPass(commandBuffers[i]);
            if(vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) throw std::runtime_error("failed to record command buffer");
        }        
    }

    void FirstApp::drawFrame(){
        uint32_t imageIndex;
        auto result = engineSwapChain.acquireNextImage(&imageIndex);

        if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) throw std::runtime_error("failed to acquire next swap chain image");

        result = engineSwapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);

        if(result != VK_SUCCESS) throw std::runtime_error("failed to present swap chain image");
    }

    void sierpinski(std::vector<EngineModel::Vertex> &vertices, int depth, glm::vec2 left, glm::vec2 right, glm::vec2 top) {
        if (depth <= 0) {
            vertices.push_back({top});
            vertices.push_back({right});
            vertices.push_back({left});
        }else {
            auto leftTop = 0.5f * (left + top);
            auto rightTop = 0.5f * (right + top);
            auto leftRight = 0.5f * (left + right);
            sierpinski(vertices, depth - 1, left, leftRight, leftTop);
            sierpinski(vertices, depth - 1, leftRight, right, rightTop);
            sierpinski(vertices, depth - 1, leftTop, rightTop, top);
        }
    }
    void FirstApp::loadModels(){
        // std::vector<EngineModel::Vertex> verticies{{{0.0f, -0.5f}}, {{0.5f, 0.5f}}, {{-0.5f, 0.5f}}};
        std::vector<EngineModel::Vertex> verticies{};
        sierpinski(verticies, 5, {-0.5f, 0.5f}, {0.5f, 0.5f}, {0.0f, -0.5f});
        engineModel = std::make_unique<EngineModel>(engineDevice, verticies);
    }   

}