module;

#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/gtc/quaternion.hpp>
#include <glm/glm.hpp>

#include "engineGameObject.hpp"

export module movementController;
import engineWindow;

namespace gears {

   export class Mouse {
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


   export class MovementController {
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

   //  ========================================== implementation ==========================================

   /**
    * float s_moveSpeed;
    *
    * void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    *    if (yoffset < 0) {
    *       --s_moveSpeed;
    *    } else if (yoffset > 0)
    *       ++s_moveSpeed;
    *  }
    */

   void MovementController::moveInPlaneYXZ(GLFWwindow* window, const Mouse& mouse, float dt, EngineGameObject& gameObject) {
      gameObject.transform.rotation = glm::angleAxis(-mouse.deltaX() * mouse.sens(), glm::vec3(0.f, -1.f, 0.f)) * gameObject.transform.rotation;
      auto pitched_rot = gameObject.transform.rotation * glm::angleAxis(mouse.deltaY() * mouse.sens(), glm::vec3(-1.f, 0.f, 0.f));
      if (glm::dot(pitched_rot * glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 1.f, 0.f)) >= 0.f) {
         gameObject.transform.rotation = pitched_rot;
      }
      gameObject.transform.rotation = glm::normalize(gameObject.transform.rotation);

      glm::vec3 translate{0.0f};
      if (glfwGetKey(window, keys.moveForward) == GLFW_PRESS) translate += glm::vec3(0.f, 0.f, 1.f);
      if (glfwGetKey(window, keys.moveBackward) == GLFW_PRESS) translate -= glm::vec3(0.f, 0.f, 1.f);
      if (glfwGetKey(window, keys.moveRight) == GLFW_PRESS) translate += glm::vec3(1.f, 0.f, 0.f);
      if (glfwGetKey(window, keys.moveLeft) == GLFW_PRESS) translate -= glm::vec3(1.f, 0.f, 0.f);

      if (glfwGetKey(window, keys.moveUp) == GLFW_PRESS) gameObject.transform.position -= glm::vec3(0.f, 1.f, 0.f) * moveSpeed * dt;
      if (glfwGetKey(window, keys.moveDown) == GLFW_PRESS) gameObject.transform.position += glm::vec3(0.f, 1.f, 0.f) * moveSpeed * dt;

      if (translate.x || translate.y || translate.z)
         gameObject.transform.position += gameObject.transform.rotation * (glm::normalize(translate) * moveSpeed * dt);
   }

   Mouse::Mouse(Window& window) : _window{window} {
      _cursor = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
   }
   Mouse::~Mouse() {
      glfwDestroyCursor(_cursor);
   }

   void Mouse::update() {
      glfwGetCursorPos(_window.getWindow(), &_x, &_y);
      _deltaX = _x - _prevX;
      _deltaY = _y - _prevY;

      _prevX = _x;
      _prevY = _y;
   }
} // namespace gears