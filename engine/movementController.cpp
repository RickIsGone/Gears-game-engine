#include "movementController.hpp"
#include "GLFW/glfw3.h"

namespace gears {
   // static float s_moveSpeed;

   // void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
   //    if (yoffset < 0) {
   //       --s_moveSpeed;
   //    } else if (yoffset > 0)
   //       ++s_moveSpeed;
   // }

   void MovementController::moveInPlaneYXZ(GLFWwindow* window, Mouse& mouse, float dt, EngineGameObject& gameObject) {
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

   Mouse::Mouse(GLFWwindow* window) : window{window} {}
   Mouse::~Mouse() {
      glfwDestroyCursor(cursor);
   }

   void Mouse::update() {
      glfwGetCursorPos(window, &posX, &posY);
      posDeltaY = posY - prevPosY;
      posDeltaX = posX - prevPosX;

      prevPosX = posX;
      prevPosY = posY;
   }
} // namespace gears
