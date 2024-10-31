#pragma once

#include "engineGameObject.hpp"
#include "engineWindow.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/gtc/quaternion.hpp>
#include <glm/glm.hpp>

namespace gears {

   class Mouse {
   public:
      Mouse(Window& window);
      ~Mouse();

      Mouse(const Mouse&) = delete;
      Mouse operator=(const Mouse&) = delete;

      void update();

      float sens() const { return _mouseSense; }
      double x() const { return _x; }
      double y() const { return _y; }
      float deltaX() const { return _deltaX; }
      float deltaY() const { return _deltaY; }
      GLFWcursor* getCursor() { return _cursor; }

   private:
      GLFWcursor* _cursor;
      Window& _window;

      double _x, _y;
      double _prevX, _prevY;
      float _deltaX, _deltaY;

      float _mouseSense = 0.0025f;
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