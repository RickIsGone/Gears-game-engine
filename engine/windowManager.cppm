module;

#include <vector>
#include <source_location>
#include <string_view>

#include <GLFW/glfw3.h>

export module engine.windowManager;
import engine.window;
import engine.logger;

namespace gears {
   export class WindowManager {
   public:
      WindowManager() = default;
      ~WindowManager();
      WindowManager(const WindowManager&) = delete;
      WindowManager& operator=(const WindowManager&) = delete;

      std::vector<Window*>& windows() { return _windows; }
      Window& window(const size_t pos, const std::source_location& location = std::source_location::current()) const;
      Window& window(const size_t pos, const std::source_location& location = std::source_location::current());

      void addWindow(std::string_view name, uint32_t width, uint32_t height, std::string_view iconPath = "assets/icons/gears_default_icon.png");

      bool idkYet();
      void closeWindow(Window& window) { glfwSetWindowShouldClose(window.getWindow(), true); }

   private:
      std::vector<Window*> _windows;
   };

   // ========================================== implementation ==========================================

   WindowManager::~WindowManager() {
      for (auto window : _windows) {
         delete window;
      }
      _windows.clear();
      glfwTerminate();
   }

   Window& WindowManager::window(size_t pos, const std::source_location& location) {
      if (_windows.empty()) throw Logger::Exception(location, "Attempted access to empty windowManager");
      if (_windows.size() <= pos) throw Logger::Exception(location, "Attempted access to out-of-bounds window index {}", pos);
      return *_windows[pos];
   }

   Window& WindowManager::window(size_t pos, const std::source_location& location) const {
      if (_windows.empty()) throw Logger::Exception(location, "Attempted access to empty windowManager");
      if (_windows.size() <= pos) throw Logger::Exception(location, "Attempted access to out-of-bounds window index {}", pos);
      return *_windows.at(pos);
   }

   void WindowManager::addWindow(std::string_view name, uint32_t width, uint32_t height, std::string_view iconPath) {
      logger->log("added new window to windowManager with parameters:\n\ttitle: \"{}\"\n\tsize: {}x{}", name, width, height);
      _windows.push_back(new Window{name, width, height, iconPath});
   }

   // void WindowManager::idkYet() {
   //    for (Window window : _windows) {
   //       if (window.shouldClose()) {
   //          delete window;
   //          window = _windows.erase(window)
   //       }
   //    }
   // }
} // namespace gears
