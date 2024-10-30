#pragma once

#include <vector>

#include "engineWindow.hpp"
#include "engineDevice.hpp"
#include "engineGameObject.hpp"
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
      Application(int width, int height, const std::string& windowName);

      // private:
      void loadGameObjects();

      Window _window;
      PhysicalDevice _device;
      Renderer _renderer;
      EngineRenderSystem _renderSystem;
      std::vector<EngineGameObject> _gameObjects;
   };
} // namespace gears