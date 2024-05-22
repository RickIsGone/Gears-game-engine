#include "Application.hpp"

#include <stdexcept>
#include <array>

namespace gears{

    Application::Application(){
        loadModels();
        createPipelineLayout();
        recreateSwapChain();
        createCommandBuffers();
    }

    Application::~Application(){
        vkDestroyPipelineLayout(engineDevice.device(), pipelineLayout, nullptr);
    }

    void Application::run(){
        while(!engineWindow.shouldClose()){
            glfwPollEvents();
            drawFrame();
        }

        vkDeviceWaitIdle(engineDevice.device()); 
    }
    
    void Application::createPipelineLayout(){
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;
        if(vkCreatePipelineLayout(engineDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) throw std::runtime_error("failed to create a pipeline layout");
    }

    void Application::createPipeline(){
        assert(engineSwapChain != nullptr && "cannot create pipeline before swap chain");
        assert(pipelineLayout != nullptr && "cannot create pipeline before pipeline layout");

        PipelineConfigInfo pipelineConfig{};
        EnginePipeline::defaultPipelineConfigInfo(pipelineConfig);
        pipelineConfig.renderPass = engineSwapChain->getRenderPass();                                                                                                
        pipelineConfig.pipelineLayout = pipelineLayout;
        enginePipeline = std::make_unique<EnginePipeline>(engineDevice, "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv", pipelineConfig); 
    }   
    
    void Application::recreateSwapChain(){
        auto extent = engineWindow.getExtent();
        while(extent.width == 0 || extent.height == 0){
            extent = engineWindow.getExtent();
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(engineDevice.device());

        if(engineSwapChain == nullptr) engineSwapChain = std::make_unique<EngineSwapChain>(engineDevice, extent);
        else{
            engineSwapChain = std::make_unique<EngineSwapChain>(engineDevice, extent, std::move(engineSwapChain));
            if(engineSwapChain->imageCount() != commandBuffers.size()){
                freeCommandBuffers();
                createCommandBuffers();
            }
        }

        // se renderpass compatibile non fare nulla
        createPipeline();
    }

    void Application::createCommandBuffers(){
        commandBuffers.resize(engineSwapChain->imageCount());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = engineDevice.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());
        
        if(vkAllocateCommandBuffers(engineDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) throw std::runtime_error("failed to allocate command buffers");     
    }

    void Application::freeCommandBuffers(){
        vkFreeCommandBuffers(engineDevice.device(), engineDevice.getCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
        commandBuffers.clear();
    }


    void Application::recordCommandBuffer(int imageIndex){
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if(vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS) throw std::runtime_error("failed to begin recording");

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = engineSwapChain->getRenderPass();
        renderPassInfo.framebuffer = engineSwapChain->getFrameBuffer(imageIndex);

        renderPassInfo.renderArea.offset = {0,0};
        renderPassInfo.renderArea.extent = engineSwapChain->getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0};

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(engineSwapChain->getSwapChainExtent().width);
        viewport.height = static_cast<float>(engineSwapChain->getSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor{{0, 0}, engineSwapChain->getSwapChainExtent()};
        vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
        vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

        enginePipeline->bind(commandBuffers[imageIndex]);
        engineModel->bind(commandBuffers[imageIndex]);
        engineModel->draw(commandBuffers[imageIndex]);

        vkCmdEndRenderPass(commandBuffers[imageIndex]);
        if(vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS) throw std::runtime_error("failed to record command buffer");
    }

    void Application::drawFrame(){
        uint32_t imageIndex;
        auto result = engineSwapChain->acquireNextImage(&imageIndex);

        if(result == VK_ERROR_OUT_OF_DATE_KHR){
            recreateSwapChain();
            return;
        }

        if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) throw std::runtime_error("failed to acquire next swap chain image");

        recordCommandBuffer(imageIndex);
        result = engineSwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
        if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || engineWindow.wasWindowResized()){
            engineWindow.resetWindowResizeFlag();
            recreateSwapChain();
            return;
        }

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
    void Application::loadModels(){
        std::vector<EngineModel::Vertex> verticies{{{0.0f, -0.5f}, {1.0, 0.0, 0.0}}, {{0.5f, 0.5f}, {0.0, 1.0, 0.0}}, {{-0.5f, 0.5f}, {0.0, 0.0, 1.0}}};
        // std::vector<EngineModel::Vertex> verticies{};
        // sierpinski(verticies, 5, {-0.5f, 0.5f}, {0.5f, 0.5f}, {0.0f, -0.5f});
        engineModel = std::make_unique<EngineModel>(engineDevice, verticies);
    }   

}