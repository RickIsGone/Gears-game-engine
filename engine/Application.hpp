#pragma once

#include <string>

import engine.windowManager;
import engine.window;
import engine.renderer;
import engine.renderSystem;
import engine.device;

namespace gears {
   class Application {
   public:
      virtual ~Application() = default;

      Application(const Application&) = delete;
      Application& operator=(const Application&) = delete;

      virtual void run() = 0;

   protected:
      Application(uint32_t width, uint32_t height, const std::string& windowName)
          : _window{windowName, width, height},
            _device{_window},
            _renderer{_window, _device},
            _renderSystem{_device, _renderer.getSwapChainRenderPass()} {}

      WindowManager _windowManager;
      Window _window;
      PhysicalDevice _device;
      Renderer _renderer;
      EngineRenderSystem _renderSystem;
   };
} // namespace gears