#include <memory>
#include <cstdlib>
#include <source_location>

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
      GRS_LOG_EXIT(e.where(), "terminating execution because of exception: {}", e.what());

   } catch (const std::exception& e) {
      GRS_LOG_EXIT(std::source_location::current(), "paused on unhandled exception: {}", e.what());

   } catch (...) {
      GRS_LOG_EXIT(std::source_location::current(), "paused on unhandled unknown exception");
   }

   return EXIT_SUCCESS;
}
