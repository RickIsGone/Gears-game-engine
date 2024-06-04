#pragma once

#include "engineGameObject.hpp"
#include "engineWindow.hpp"

namespace gears{

    class MovementController{
    public:
        struct KeyMappings{
            int moveLeft = GLFW_KEY_A;
            int moveRight = GLFW_KEY_D;
            int moveForward = GLFW_KEY_W;
            int moveBackward = GLFW_KEY_S;
            int moveUp = GLFW_KEY_E;
            int moveDown = GLFW_KEY_Q;
        };

        void moveInPlaneYXZ(GLFWwindow* window, GLFWcursor* cursor, float dt, EngineGameObject & gameObject);

        float mouse_sense = 0.0025f;

        KeyMappings keys{};
        
        float moveSpeed{10.0f};

        double prevPosX;
        double prevPosY;
    };
}