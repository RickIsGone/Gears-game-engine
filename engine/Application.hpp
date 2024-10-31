#pragma once

#include "engineWindow.hpp"
#include "engineDevice.hpp"
#include "engineRenderer.hpp"
#include "engineRenderSystem.hpp"

namespace gears {
   class Application {
   public:
      virtual ~Application() = default;

      Application(const Application&) = delete;
      Application& operator=(const Application&) = delete;

      virtual void run() = 0;

   protected:
      Application(int width, int height, const std::string& windowName)
          : _window{width, height, windowName},
            _device{_window},
            _renderer{_window, _device},
            _renderSystem{_device, _renderer.getSwapChainRenderPass()} {}

      Window _window;
      PhysicalDevice _device;
      Renderer _renderer;
      EngineRenderSystem _renderSystem;
   };
} // namespace gears