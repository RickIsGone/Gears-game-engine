#pragma once

#include "engineGameObject.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/gtc/quaternion.hpp>
#include <glm/glm.hpp>

namespace gears {

   class MovementController {
   public:
      struct KeyMappings {
         int moveLeft = GLFW_KEY_A;
         int moveRight = GLFW_KEY_D;
         int moveForward = GLFW_KEY_W;
         int moveBackward = GLFW_KEY_S;
         int tiltRight = GLFW_KEY_E;
         int tiltLeft = GLFW_KEY_Q;
      };

      void moveInPlaneYXZ(GLFWwindow* window, GLFWcursor* cursor, float dt, EngineGameObject& gameObject);

      float mouseSense = 0.0025f;

      KeyMappings keys{};

      float moveSpeed = 10.f;

      double prevPosX;
      double prevPosY;
   };

} // namespace gears