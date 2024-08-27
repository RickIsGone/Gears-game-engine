#pragma once

#include <vector>

#include "engineWindow.hpp"
#include "engineDevice.hpp"
#include "engineGameObject.hpp"
#include "engineRenderer.hpp"

namespace gears {

   class Application {
   public:
      static constexpr int WIDTH = 1280;
      static constexpr int HEIGHT = 720;

      Application();
      ~Application();

      Application(const Application&) = delete;
      Application& operator=(const Application&) = delete;

      void run();

   private:
      void loadGameObjects();

      EngineWindow engineWindow{WIDTH, HEIGHT, "Gears engine goes brrrrrrrrrrrr"};
      EngineDevice engineDevice{engineWindow};
      EngineRenderer engineRenderer{engineWindow, engineDevice};
      std::vector<EngineGameObject> gameObjects;
   };
} // namespace gears