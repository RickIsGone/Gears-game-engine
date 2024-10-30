#pragma once
#include "Application.hpp"

namespace gears {
   class Engine : public Application {
   public:
      Engine(int width, int height, const std::string& windowName)
          : Application(width, height, windowName) {}

      ~Engine() = default;
      void run();
   };
} // namespace gears
