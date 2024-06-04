#include "movementController.hpp"
#include "engineWindow.hpp"

namespace gears{

    void MovementController::moveInPlaneYXZ(GLFWwindow* window, GLFWcursor* cursor, float dt, EngineGameObject& gameObject){
        double posX, posY;
        glfwGetCursorPos(window, &posY, &posX);
        double deltaY = posY - prevPosY;
        double deltaX = posX - prevPosX;

        glm::vec3 rotate{0.0f};
        rotate.y += deltaY;
        rotate.x -= deltaX;

        gameObject.transform.rotation += mouse_sense * rotate;

        gameObject.transform.rotation.x = glm::clamp(gameObject.transform.rotation.x, -1.5f, 1.5f);
        gameObject.transform.rotation.y = glm::mod(gameObject.transform.rotation.y, glm::two_pi<float>());

        float yaw = gameObject.transform.rotation.y;
        double pitch = -gameObject.transform.rotation.x;
        const glm::vec3 forwardDir{sin(yaw), pitch, cos(yaw)};
        const glm::vec3 rightDir{forwardDir.z, 0.0f, -forwardDir.x};
        const glm::vec3 upDir{0.0f, -1.0f, 0.0f};

        glm::vec3 moveDir{0.0f};
        if(glfwGetKey(window, keys.moveForward) == GLFW_PRESS) moveDir += forwardDir;
        if(glfwGetKey(window, keys.moveBackward) == GLFW_PRESS) moveDir -= forwardDir;
        if(glfwGetKey(window, keys.moveRight) == GLFW_PRESS) moveDir += rightDir;
        if(glfwGetKey(window, keys.moveLeft) == GLFW_PRESS) moveDir -= rightDir;
        if(glfwGetKey(window, keys.moveUp) == GLFW_PRESS) moveDir += upDir;
        if(glfwGetKey(window, keys.moveDown) == GLFW_PRESS) moveDir -= upDir;

        if(glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon())
            gameObject.transform.translation += moveSpeed * dt * glm::normalize(moveDir);

        prevPosX = posX;
        prevPosY = posY;
    }

}
