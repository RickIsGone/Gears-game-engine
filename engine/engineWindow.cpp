#include <format>
#include <string>

#include "engineWindow.hpp"
#include "GLFW/glfw3.h"
#include "logger.hpp"

#include "vendor/stb_image/stb_image.h"

namespace gears {

   Window::Window(uint32_t width, uint32_t height, const std::string& name) : _width{width}, _height{height}, _windowName{name} {
      glfwInit();
      glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
      glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

      _window = glfwCreateWindow(_width, _height, _windowName.c_str(), nullptr, nullptr);
      glfwSetWindowUserPointer(_window, this);
      glfwSetFramebufferSizeCallback(_window, _frameBufferResizedCallback);
   }

   Window::~Window() {
      glfwDestroyWindow(_window);
      glfwTerminate();
   }

   void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) {
      if (glfwCreateWindowSurface(instance, _window, nullptr, surface) != VK_SUCCESS)
         throw Logger::Exception("failed to create window surface");
   }

   void Window::_frameBufferResizedCallback(GLFWwindow* window, int width, int height) {
      auto engineWindow = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
      engineWindow->_frameBufferResized = true;
      engineWindow->_width = width;
      engineWindow->_height = height;
      logger->logTrace(std::format("Window resized to {}x{}", width, height));
   }

   // suggested icon sizes: 16x16, 32x32, 48x48.
   void Window::setIcon(const std::string& iconPath) {
      GLFWimage images[1];
      images[0].pixels = stbi_load(iconPath.c_str(), &images[0].width, &images[0].height, 0, 4);
      glfwSetWindowIcon(_window, 1, images);
      stbi_image_free(images[0].pixels);
   }

   void Window::rename(const std::string& newName) {
      _windowName = newName;
      glfwSetWindowTitle(_window, _windowName.c_str());
   }

} // namespace gears