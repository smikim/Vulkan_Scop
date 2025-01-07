#pragma once

#include "../Application/GlfwWindow.hpp"
#include <vector>

namespace scop
{
    class ScopObject;

    class KeyboardMovementController
    {
    public:

        struct KeyMappings {
            int moveLeft = GLFW_KEY_A;
            int moveRight = GLFW_KEY_D;
            int moveForward = GLFW_KEY_W;
            int moveBackward = GLFW_KEY_S;
            int moveUp = GLFW_KEY_E;
            int moveDown = GLFW_KEY_Q;
            int lookLeft = GLFW_KEY_LEFT;
            int lookRight = GLFW_KEY_RIGHT;
            int lookUp = GLFW_KEY_UP;
            int lookDown = GLFW_KEY_DOWN;
        };

        struct MoveKeyMappings {
            int moveLeftInX = GLFW_KEY_COMMA;
            int moveRightInX = GLFW_KEY_PERIOD;
            int moveLeftInY = GLFW_KEY_LEFT;
            int moveRightInY = GLFW_KEY_RIGHT;
            int moveUpInZ = GLFW_KEY_UP;
            int moveDownInZ = GLFW_KEY_DOWN;
            int spinOnX = GLFW_KEY_1;
            int spinOnY = GLFW_KEY_2;
            int spinOnZ = GLFW_KEY_3;
            int resetAll = GLFW_KEY_4;
        };

        struct ColoringKeyMapping{
            uint32_t    switchKey = GLFW_KEY_8;
        };

        void moveInPlaneXZ(GLFWwindow* window, float dt, ScopObject& gameObject);
        void myMoveInPlaneXZ(GLFWwindow* window, float dt, ScopObject& gameObject);
        
        void moveObjects(GLFWwindow* window, float dt, std::vector<ScopObject*>& gameObjects);

        void switchColoring(GLFWwindow* window, std::vector<ScopObject*>& gameObjects);

        KeyMappings keys{};
        MoveKeyMappings moves{};
        ColoringKeyMapping coloring;

        float moveSpeed{ 3.f };
        float lookSpeed{ 1.5f };
    };


}
