module;

#include <string_view>
#include <string>

#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "vendor/stb_image/stb_image.h"

export module engine.window;
import engine.logger;

namespace gears {

   export class Window {
   public:
      Window(std::string_view name, uint32_t width, uint32_t height, std::string_view iconPath = "assets/icons/gears_default_icon.png");
      ~Window();

      Window(const Window&) = delete;
      Window& operator=(const Window&) = delete;


      bool shouldClose() { return glfwWindowShouldClose(_window); }
      VkExtent2D getExtent() { return VkExtent2D{_width, _height}; }
      bool wasWindowResized() { return _frameBufferResized; }
      void resetWindowResizeFlag() { _frameBufferResized = false; }
      GLFWwindow* getWindow() const { return _window; }
      std::string title() { return _title; }
      uint32_t id() const { return _id; }

      void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

      void setVisibility(bool visibiliy);
      void setIcon(std::string_view iconPath);
      void setTitle(std::string_view newTitle);

   private:
      static void _frameBufferResizedCallback(GLFWwindow* _window, int width, int height);

      GLFWwindow* _window;

      uint32_t _width;
      uint32_t _height;
      bool _frameBufferResized = false;
      bool _isVisible;

      std::string _title;

      uint32_t _id;
   };

   // ========================================== implementation ==========================================


   Window::Window(std::string_view name, uint32_t width, uint32_t height, std::string_view iconPath) : _title{name}, _width{width}, _height{height} {
      static uint32_t currentId = 0;
      _id = currentId++;

      glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
      glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

      _window = glfwCreateWindow(_width, _height, _title.c_str(), nullptr, nullptr);
      glfwSetWindowUserPointer(_window, this);
      glfwSetFramebufferSizeCallback(_window, _frameBufferResizedCallback);

      setIcon(iconPath);
   }

   Window::~Window() {
      glfwDestroyWindow(_window);
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
      logger->logTrace("Window resized to {}x{}", width, height);
   }

   void Window::setVisibility(bool visibility) {
      if (visibility == _isVisible) return;
      if (visibility) {
         glfwShowWindow(_window);

      } else {
         glfwHideWindow(_window);
      }
      logger->log("window id: {} visibility set to {}", _id, visibility);
   }

   // suggested icon sizes: 16x16, 32x32, 48x48.
   void Window::setIcon(std::string_view iconPath) {
      GLFWimage images[1];
      images[0].pixels = stbi_load(iconPath.data(), &images[0].width, &images[0].height, 0, 4);
      glfwSetWindowIcon(_window, 1, images);
      stbi_image_free(images[0].pixels);
   }

   void Window::setTitle(std::string_view newTitle) {
      _title = newTitle;
      glfwSetWindowTitle(_window, _title.c_str());
   }
} // namespace gears