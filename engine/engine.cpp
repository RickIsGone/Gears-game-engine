#include <chrono>

#include <vulkan/vulkan.hpp>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <GLFW/glfw3.h>

#include "engine.hpp"
import movementController;
import engine.camera;
import engine.logger;
import engineModel;

namespace gears {

   Engine::Engine(uint32_t width, uint32_t height, const std::string& windowName) : Application(width, height, windowName) {
      _loadGameObjects();
   }

   static void mouseCallback(GLFWwindow* window, int button, int action, int mods) {
      if (button == GLFW_MOUSE_BUTTON_RIGHT) {
         if (action == GLFW_PRESS) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            if (glfwRawMouseMotionSupported())
               glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
         } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            if (glfwRawMouseMotionSupported())
               glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
         }
      }
   }

   void Engine::run() {
      logger->log("starting engine");
      // _windowManager.addWindow("Gears engine goes brrrrrrrrrrrr", 1280, 720);
      // _windowManager[0].setVisibility(false);
      // _windowManager.at(0).setVisibility(true);
      // _windowManager.closeWindow(1);

      EngineCamera camera{};

      auto viewerObject = EngineGameObject::createGameObject();

      MovementController cameraController{};
      Mouse mouse{_window};
      mouse.update();

      auto prevTime = std::chrono::high_resolution_clock::now();

      while (!_window.shouldClose()) {
         glfwPollEvents();
         mouse.update();
         _windowManager.removeClosedWindows();

         auto currentTime = std::chrono::high_resolution_clock::now();
         float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - prevTime).count();
         prevTime = currentTime;

         glfwSetMouseButtonCallback(_window.getWindow(), mouseCallback); // se RMB e' premuto muove la camera senno' muove il mouse
         if (glfwGetMouseButton(_window.getWindow(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
            cameraController.moveInPlaneYXZ(_window.getWindow(), mouse, deltaTime, viewerObject);
            camera.setViewYXZ(viewerObject.transform.position, viewerObject.transform.rotation);
         }

         float aspect = _renderer.getAspectRatio();
         camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 50.f);

         if (auto commandBuffer = _renderer.beginFrame()) {
            _renderer.beginSwapChainRenderPass(commandBuffer);
            _renderSystem.renderGameObjects(commandBuffer, _gameObjects, camera);
            _renderer.endSwapChainRenderPass(commandBuffer);
            _renderer.endFrame();
         }
      }

      vkDeviceWaitIdle(_device.device());
      logger->log("terminating engine");
   }

   void Engine::_loadGameObjects() {
      std::shared_ptr<EngineModel> model = EngineModel::createModelFromFile(_device, "flat_vase.obj");


      auto flatVase = EngineGameObject::createGameObject();
      flatVase.model = model;
      flatVase.transform.position = {-.5f, .5f, 2.5f};
      flatVase.transform.scale = {3.f, 1.5f, 3.f};
      _gameObjects.push_back(std::move(flatVase));
      logger->log("Loaded model \"flat_vase.obj\"");

      model = EngineModel::createModelFromFile(_device, "smooth_vase.obj");
      auto smoothVase = EngineGameObject::createGameObject();
      smoothVase.model = model;
      smoothVase.transform.position = {.5f, .5f, 2.5f};
      smoothVase.transform.scale = {3.f, 1.5f, 3.f};
      _gameObjects.push_back(std::move(smoothVase));
      logger->log("Loaded model \"smooth_vase.obj\"");
   }
} // namespace gears