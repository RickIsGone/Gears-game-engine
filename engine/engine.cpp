#include <chrono>

#include "engine.hpp"
#include "engineCamera.hpp"
#include "movementController.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace gears {

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
      EngineCamera camera{};

      auto viewerObject = EngineGameObject::createGameObject();

      MovementController cameraController{};
      Mouse mouse{_window.getWindow()};
      mouse.update();

      auto prevTime = std::chrono::high_resolution_clock::now();

      while (!_window.shouldClose()) {
         glfwPollEvents();
         mouse.update();

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
   }
} // namespace gears