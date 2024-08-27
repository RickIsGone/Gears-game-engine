#include "movementController.hpp"

namespace gears {

   void MovementController::moveInPlaneYXZ(GLFWwindow* window, GLFWcursor* cursor, float dt, EngineGameObject& gameObject) {
      double posX, posY;
      glfwGetCursorPos(window, &posY, &posX);
      float deltaY = posY - prevPosY;
      float deltaX = posX - prevPosX;

      gameObject.transform.rotation = glm::angleAxis(-deltaY * mouseSense, glm::vec3(0.f, -1.f, 0.f)) * gameObject.transform.rotation;
      gameObject.transform.rotation = gameObject.transform.rotation * glm::angleAxis(deltaX * mouseSense, glm::vec3(-1.f, 0.f, 0.f));
      gameObject.transform.rotation = glm::normalize(gameObject.transform.rotation);

      auto forwardDir = gameObject.transform.rotation * glm::vec3(0.f, 0.f, 1.f);
      auto rightDir = gameObject.transform.rotation * glm::vec3(1.f, 0.f, 0.f);
      auto upDir = gameObject.transform.rotation * glm::vec3(0.f, 1.f, 0.f);

      glm::vec3 moveDir{0.0f};
      if (glfwGetKey(window, keys.moveForward) == GLFW_PRESS) moveDir += forwardDir;
      if (glfwGetKey(window, keys.moveBackward) == GLFW_PRESS) moveDir -= forwardDir;
      if (glfwGetKey(window, keys.moveRight) == GLFW_PRESS) moveDir += rightDir;
      if (glfwGetKey(window, keys.moveLeft) == GLFW_PRESS) moveDir -= rightDir;
      if (glfwGetKey(window, keys.moveUp) == GLFW_PRESS) moveDir += upDir;
      if (glfwGetKey(window, keys.moveDown) == GLFW_PRESS) moveDir -= upDir;

      if (moveDir.x || moveDir.y || moveDir.z)
         gameObject.transform.translation += gameObject.transform.rotation * (glm::normalize(moveDir) * moveSpeed * dt);

      prevPosX = posX;
      prevPosY = posY;
   }

} // namespace gears
