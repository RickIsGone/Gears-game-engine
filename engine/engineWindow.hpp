#pragma once

#include <string>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace gears {

   class Window {
   public:
      Window(uint32_t width, uint32_t height, const std::string& name);
      ~Window();

      Window(const Window&) = delete;
      Window& operator=(const Window&) = delete;


      bool shouldClose() { return glfwWindowShouldClose(_window); }
      VkExtent2D getExtent() { return VkExtent2D{_width, _height}; }
      bool wasWindowResized() { return _frameBufferResized; }
      void resetWindowResizeFlag() { _frameBufferResized = false; }
      GLFWwindow* getWindow() const { return _window; }

      void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

      void setIcon(const std::string& iconPath);
      void rename(const std::string& newName);

   private:
      static void _frameBufferResizedCallback(GLFWwindow* _window, int width, int height);

      uint32_t _width;
      uint32_t _height;
      bool _frameBufferResized = false;

      std::string _windowName;
      GLFWwindow* _window;
   };
} // namespace gears