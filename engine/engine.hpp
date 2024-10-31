#pragma once
#include <vector>

#include "Application.hpp"
#include "engineGameObject.hpp"

namespace gears {
   class Engine : public Application {
   public:
      Engine(int width, int height, const std::string& windowName);
      ~Engine() = default;

      void run();

   private:
      void _loadGameObjects();
      std::vector<EngineGameObject> _gameObjects;
   };
} // namespace gears
