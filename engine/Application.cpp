#include "Application.hpp"
#include "engineRenderSystem.hpp"

#include <stdexcept>
#include <array>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace gears{

    Application::Application(){
        loadGameObjects();
    }

    Application::~Application(){}

    void Application::run(){
        EngineRenderSystem engineRenderSystem{engineDevice, engineRenderer.getSwapChainRenderPass()};
        
        while(!engineWindow.shouldClose()){
            glfwPollEvents();

            if(auto commandBuffer = engineRenderer.beginFrame()){
                engineRenderer.beginSwapChainRenderPass(commandBuffer);
                engineRenderSystem.renderGameObjects(commandBuffer, gameObjects);
                engineRenderer.endSwapChainRenderPass(commandBuffer);
                engineRenderer.endFrame();
            }
            
        }

        vkDeviceWaitIdle(engineDevice.device()); 
    }

    void Application::loadGameObjects(){
        std::vector<EngineModel::Vertex> vertices{
        {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};
        auto engineModel = std::make_shared<EngineModel>(engineDevice, vertices);
    
        std::vector<glm::vec3> colors{
        {1.f, .7f, .73f},
        {1.f, .87f, .73f},
        {1.f, 1.f, .73f},
        {.73f, 1.f, .8f},
        {.73, .88f, 1.f}};

        for (auto& color : colors) {
          color = glm::pow(color, glm::vec3{2.2f});
        }
        for (int i = 0; i < 40; i++) {
            auto triangle = EngineGameObject::createGameObject();
            triangle.model = engineModel;
            triangle.transform2d.scale = glm::vec2(.5f) + i * 0.025f;
            triangle.transform2d.rotation = i * glm::pi<float>() * .025f;
            triangle.color = colors[i % colors.size()];
            gameObjects.push_back(std::move(triangle));
        }
    }   

}
