#include <format>

#include "engineWindow.hpp"
#include "logger.hpp"

#include "vendor/stb_image/stb_image.h"

namespace gears {

   Window::Window(int w, int h, const std::string& name) : _width{w}, _height{h}, _windowName{name} {
      initWindow();
   }

   Window::~Window() {
      glfwDestroyWindow(_window);
      glfwTerminate();
   }

   void Window::initWindow() {
      glfwInit();
      glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
      glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

      _window = glfwCreateWindow(_width, _height, _windowName.c_str(), nullptr, nullptr);
      glfwSetWindowUserPointer(_window, this);
      glfwSetFramebufferSizeCallback(_window, frameBufferResizedCallback);

      GLFWimage images[1];
      images[0].pixels = stbi_load("assets/icons/gears-icon.png", &images[0].width, &images[0].height, 0, 4);
      glfwSetWindowIcon(_window, 1, images);
      stbi_image_free(images[0].pixels);
   }

   void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) {
      if (glfwCreateWindowSurface(instance, _window, nullptr, surface) != VK_SUCCESS)
         throw Logger::Exception("failed to create window surface");
   }

   void Window::frameBufferResizedCallback(GLFWwindow* window, int width, int height) {
      auto engineWindow = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
      engineWindow->_frameBufferResized = true;
      engineWindow->_width = width;
      engineWindow->_height = height;
      logger->log(std::format("Window resized to {}x{}", width, height));
   }

} // namespace gears