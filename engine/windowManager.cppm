module;

#include <vector>
#include <algorithm>
#include <source_location>
#include <string_view>
#include <memory>

#include <GLFW/glfw3.h>

export module engine.windowManager;
import engine.window;
import engine.logger;

namespace gears {
   export class WindowManager {
   public:
      WindowManager();
      ~WindowManager();

      WindowManager(const WindowManager&) = delete;
      WindowManager& operator=(const WindowManager&) = delete;


      std::vector<std::unique_ptr<Window>>& windows() { return _windows; }
      Window& operator[](const size_t pos);
      Window& at(const size_t pos, const std::source_location& location = std::source_location::current()) const;

      void addWindow(std::string_view name, uint32_t width, uint32_t height, std::string_view iconPath = "assets/icons/gears_default_icon.png");

      void removeClosedWindows();
      void closeWindow(uint32_t id, const std::source_location& location = std::source_location::current());

   private:
      std::vector<std::unique_ptr<Window>> _windows;
   };

   // ========================================== implementation ==========================================
   WindowManager::WindowManager() {
      glfwInit();
   }

   WindowManager::~WindowManager() {
      _windows.clear();
      glfwTerminate();
   }

   Window& WindowManager::operator[](const size_t pos) {
      if (_windows.empty()) throw Logger::Exception("Attempted access to empty windowManager");
      if (_windows.size() <= pos) throw Logger::Exception("Attempted access to out-of-bounds window index: {}", pos);
      return *_windows[pos];
   }

   Window& WindowManager::at(size_t pos, const std::source_location& location) const {
      if (_windows.empty()) throw Logger::Exception(location, "Attempted access to empty windowManager");
      if (_windows.size() <= pos) throw Logger::Exception(location, "Attempted access to out-of-bounds window index: {}", pos);
      return *_windows.at(pos);
   }

   void WindowManager::addWindow(std::string_view name, uint32_t width, uint32_t height, std::string_view iconPath) {
      _windows.push_back(std::make_unique<Window>(name, width, height, iconPath));
      logger->log("added new window to windowManager with parameters:\n\tid: {}\n\ttitle: \"{}\"\n\tsize: {}x{}", _windows.back()->id(), name, width, height);
   }

   void WindowManager::closeWindow(uint32_t id, const std::source_location& location) {
      auto newEnd = std::remove_if(_windows.begin(), _windows.end(), [id](const std::unique_ptr<Window>& window) { return window->id() == id; });

      if (newEnd != _windows.end()) {
         _windows.erase(newEnd, _windows.end());
         _windows.shrink_to_fit();
         logger->log("removed window id: {} from windowManager", id);

      } else {
         throw Logger::Exception(location, "no window id: {} found in windowManager", id);
      }
   }

   void WindowManager::removeClosedWindows() {
      auto newEnd = std::remove_if(_windows.begin(), _windows.end(), [](const std::unique_ptr<Window>& window) {
         if (window->shouldClose()) {
            logger->log("removed window id: {} from windowManager", window->id());
            return true;
         }
         return false;
      });
      _windows.erase(newEnd, _windows.end());
      _windows.shrink_to_fit();
   }
} // namespace gears
