﻿#include <iostream>
#include <cstdlib>
#include <stdexcept>

#include "Application.hpp"
#include "macro.hpp"


int main(int argc, char* argv[]) {
   gears::Application app{};

   try {
      app.run();
   } catch (const std::exception& e) {
      std::cerr << e.what() << '\n';
      GEARS_DEBUG_WAIT;
      return EXIT_FAILURE;
   }

   return EXIT_SUCCESS;
}
