#pragma once

#include <vector>

#include "engineWindow.hpp"
#include "engineDevice.hpp"
#include "engineGameObject.hpp"
#include "engineRenderer.hpp"

namespace gears {
   static constexpr int WIDTH = 1280;
   static constexpr int HEIGHT = 720;

   class Application {
   public:
      Application();
      virtual ~Application() = default;

      Application(const Application&) = delete;
      Application& operator=(const Application&) = delete;

      virtual void run();

   private:
      void loadGameObjects();

      Window _window{WIDTH, HEIGHT, "Gears engine goes brrrrrrrrrrrr"};
      PhysicalDevice _device{_window};
      Renderer _renderer{_window, _device};
      std::vector<EngineGameObject> _gameObjects;
   };
} // namespace gears