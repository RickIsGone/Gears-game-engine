#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace gears{

    class EngineWindow{
    public:
        EngineWindow(int width, int height, std::string windowName);
        ~EngineWindow();
        
        EngineWindow(const EngineWindow &) = delete;
        EngineWindow &operator = (const EngineWindow &) = delete;

        
        bool shouldClose() { return glfwWindowShouldClose(window); }
        VkExtent2D getExtent() { return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; }
        bool wasWindowResized() { return frameBufferResized; }
        void resetWindowResizeFlag() { frameBufferResized = false; }

        void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
        
    private:
        static void frameBufferResizedCallback(GLFWwindow *window, int width, int height);
        void initWindow();

        int width;
        int height;
        bool frameBufferResized = false;

        std::string windowName;
        GLFWwindow* window;
    };
}