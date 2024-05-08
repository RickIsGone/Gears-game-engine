#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace engine{

    class EngineWindow{
    public:
        EngineWindow(int width, int height, std::string windowName);
        ~EngineWindow();
        
        EngineWindow(const EngineWindow &) = delete;
        EngineWindow &operator = (const EngineWindow) = delete;

        
        bool shouldClose();
        
        void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
        
    private:
        void initWindow();

        const int width;
        const int height;

        std::string windowName;
        GLFWwindow* window;
    };
}