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

    std::unique_ptr<EngineModel> createCubeModel(EngineDevice& device, glm::vec3 offset) {
    EngineModel::Data modelData{};
    modelData.vertices = {
      // left face (white)
      {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
      {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
      {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
      {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
 
      // right face (yellow)
      {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
      {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
      {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
      {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
 
      // top face (orange, remember y axis points down)
      {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
      {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
      {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
      {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
 
      // bottom face (red)
      {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
      {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
      {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
      {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
 
      // nose face (blue)
      {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
      {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
      {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
      {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
 
      // tail face (green)
      {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
      {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
      {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
      {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
  };
  for (auto& v : modelData.vertices) {
    v.position += offset;
  }
 
  modelData.indices = {0,  1,  2,  0,  3,  1,  4,  5,  6,  4,  7,  5,  8,  9,  10, 8,  11, 9,
                          12, 13, 14, 12, 15, 13, 16, 17, 18, 16, 19, 17, 20, 21, 22, 20, 23, 21};
 
  return std::make_unique<EngineModel>(device, modelData);
}

    void Application::loadGameObjects(){
        std::shared_ptr<EngineModel> engineModel = createCubeModel(engineDevice, {0.0f ,0.0f, 0.0f});

        auto cube = EngineGameObject::createGameObject();
        cube.model = engineModel;
        cube.transform.translation = {0.0f, 0.0f, 2.0f};
        cube.transform.scale = {0.5f, 0.5f, 0.5f};

        gameObjects.push_back(std::move(cube));
    }   

}
