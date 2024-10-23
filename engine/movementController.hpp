#pragma once

#include "engineGameObject.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/gtc/quaternion.hpp>
#include <glm/glm.hpp>

namespace gears {

   class Mouse {
   public:
      Mouse(GLFWwindow* window);
      ~Mouse();

      Mouse(const Mouse&) = delete;
      Mouse operator=(const Mouse&) = delete;

      void update();

      float sens() const { return mouseSense; }
      double x() const { return posX; }
      double y() const { return posY; }
      float deltaX() const { return posDeltaX; }
      float deltaY() const { return posDeltaY; }
      GLFWcursor* getCursor() { return cursor; }

   private:
      GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
      GLFWwindow* window;

      double posX, posY;
      double prevPosX, prevPosY;
      float posDeltaX, posDeltaY;

      float mouseSense = 0.0025f;
   };


   class MovementController {
   public:
      struct KeyMappings {
         int moveLeft = GLFW_KEY_A;
         int moveRight = GLFW_KEY_D;
         int moveForward = GLFW_KEY_W;
         int moveBackward = GLFW_KEY_S;
         int moveUp = GLFW_KEY_E;
         int moveDown = GLFW_KEY_Q;
      };

      void moveInPlaneYXZ(GLFWwindow* window, const Mouse& mouse, float dt, EngineGameObject& gameObject);

      KeyMappings keys{};

      float moveSpeed = 10.f;
   };


} // namespace gears