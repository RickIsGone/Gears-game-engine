#pragma once

#include <string>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace gears {

   class Window {
   public:
      Window(int width, int height, const std::string& windowName);
      ~Window();

      Window(const Window&) = delete;
      Window& operator=(const Window&) = delete;


      bool shouldClose() { return glfwWindowShouldClose(_window); }
      VkExtent2D getExtent() { return {static_cast<uint32_t>(_width), static_cast<uint32_t>(_height)}; }
      bool wasWindowResized() { return _frameBufferResized; }
      void resetWindowResizeFlag() { _frameBufferResized = false; }
      GLFWwindow* getWindow() const { return _window; }

      void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

      void setIcon(const std::string& iconPath);

   private:
      static void frameBufferResizedCallback(GLFWwindow* _window, int width, int height);

      int _width;
      int _height;
      bool _frameBufferResized = false;

      std::string _windowName;
      GLFWwindow* _window;
   };
} // namespace gears