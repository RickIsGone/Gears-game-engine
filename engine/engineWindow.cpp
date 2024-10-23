#include "engineWindow.hpp"
#include "logger.hpp"

#include "vendor/stb_image/stb_image.h"

namespace gears {

   EngineWindow::EngineWindow(int w, int h, std::string name) : width{w}, height{h}, windowName{name} {
      initWindow();
   }

   EngineWindow::~EngineWindow() {
      glfwDestroyWindow(window);
      glfwTerminate();
   }

   void EngineWindow::initWindow() {
      glfwInit();
      glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
      glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

      window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
      glfwSetWindowUserPointer(window, this);
      glfwSetFramebufferSizeCallback(window, frameBufferResizedCallback);

      GLFWimage images[1];
      images[0].pixels = stbi_load("assets/icons/gears-icon.png", &images[0].width, &images[0].height, 0, 4);
      glfwSetWindowIcon(window, 1, images);
      stbi_image_free(images[0].pixels);
   }

   void EngineWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) {
      if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
         throw Logger::Exception("failed to create window surface");
   }

   void EngineWindow::frameBufferResizedCallback(GLFWwindow* window, int width, int height) {
      auto engineWindow = reinterpret_cast<EngineWindow*>(glfwGetWindowUserPointer(window));
      engineWindow->frameBufferResized = true;
      engineWindow->width = width;
      engineWindow->height = height;
      logger->log("Window resized to " + std::to_string(width) + "x" + std::to_string(height));
   }

} // namespace gears