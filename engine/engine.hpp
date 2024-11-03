#pragma once
#include <vector>

#include "Application.hpp"
#include "engineGameObject.hpp"

namespace gears {
   class Engine : public Application {
   public:
      Engine(uint32_t width, uint32_t height, const std::string& windowName);
      ~Engine() = default;

      void run();

   private:
      void _loadGameObjects();
      std::vector<EngineGameObject> _gameObjects;
   };
} // namespace gears
