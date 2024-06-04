#include "Application.hpp"
#include "engineRenderSystem.hpp"
#include "engineCamera.hpp"
#include "movementController.hpp"

#include <stdexcept>
#include <array>
#include <chrono>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// #define MAX_FRAME_TIME 

namespace gears{

    Application::Application(){
        loadGameObjects();
    }

    Application::~Application(){}

    void Application::run(){
        EngineRenderSystem engineRenderSystem{engineDevice, engineRenderer.getSwapChainRenderPass()};
        EngineCamera camera{};
        camera.setViewTarget(glm::vec3{-1.0f, -2.0f, 2.0f}, glm::vec3{0.0f, 0.0f, 2.0f});

        auto viewerObject = EngineGameObject::createGameObject();

        MovementController cameraController{};
        GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
        glfwGetCursorPos(engineWindow.getWindow(), &cameraController.prevPosX, &cameraController.prevPosY);

        auto currentTime = std::chrono::high_resolution_clock::now();

        while(!engineWindow.shouldClose()){
            glfwPollEvents();

            auto newTime = std::chrono::high_resolution_clock::now();
            float deltaTime = std::chrono::duration<float,std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;
            // deltaTime = glm::min(deltaTime, MAX_FRAME_TIME);
            
            if(glfwGetMouseButton(engineWindow.getWindow(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS){
                glfwSetInputMode(engineWindow.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                if (glfwRawMouseMotionSupported()) 
                    glfwSetInputMode(engineWindow.getWindow(), GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

                cameraController.moveInPlaneYXZ(engineWindow.getWindow(), cursor, deltaTime, viewerObject);
                camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

            }else {
                glfwSetInputMode(engineWindow.getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                if (glfwRawMouseMotionSupported()) 
                    glfwSetInputMode(engineWindow.getWindow(), GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
            }

            float aspect = engineRenderer.getAspectRatio();
            camera.setPerspectiveProjection(glm::radians(50.0f), aspect, 0.1f, 10.0f);


            if(auto commandBuffer = engineRenderer.beginFrame()){
                engineRenderer.beginSwapChainRenderPass(commandBuffer);
                engineRenderSystem.renderGameObjects(commandBuffer, gameObjects, camera);
                engineRenderer.endSwapChainRenderPass(commandBuffer);
                engineRenderer.endFrame();
            }
        }

        glfwDestroyCursor(cursor);
        vkDeviceWaitIdle(engineDevice.device()); 
    }


    void Application::loadGameObjects(){
        std::shared_ptr<EngineModel> engineModel = EngineModel::createModelFromFile(engineDevice, "fish.obj");

        auto gameObject = EngineGameObject::createGameObject();
        gameObject.model = engineModel;
        gameObject.transform.translation = {0.0f, 0.0f, 2.0f};
        gameObject.transform.scale = {0.5f, 0.5f, 0.5f};

        gameObjects.push_back(std::move(gameObject));
    }   

}
