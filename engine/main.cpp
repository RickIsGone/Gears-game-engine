#include <cassert>
#include <format>
#include <source_location>
#include <memory>
#include <cstdlib>

#include "engine.hpp"
#include "main.hpp"
#include "macro.hpp"
import engine.logger;

int main(int argc, char** argv) {
   gears::Logger logger{};

   try {
      std::unique_ptr<gears::Engine> app = std::make_unique<gears::Engine>(1280, 720, "Gears engine goes brrrrrrrrrrrr");
      app->run();

   } catch (const gears::Logger::Exception& e) {
      GRS_LOG_EXIT(std::format("terminating execution because of exception: {}", e.what()), e.where());

   } catch (const std::exception& e) {
      GRS_LOG_EXIT(std::format("paused on unhandled exception: {}", e.what()), std::source_location::current());

   } catch (...) {
      GRS_LOG_EXIT("paused on unhandled unkown exception", std::source_location::current());
   }

   return EXIT_SUCCESS;
}
