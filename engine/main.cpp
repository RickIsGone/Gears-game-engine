#include <format>

#include "engine.hpp"
#include "macro.hpp"
#include "logger.hpp"

/* creato logger globale in logger.hpp */
int main(int argc, char** argv) {
   try {
      gears::Logger logger{};

      try {
         std::unique_ptr<gears::Engine> app = std::make_unique<gears::Engine>(1280, 720, "Gears engine goes brrrrrrrrrrrr"); /* sopra 1KB allocare heap */
         app->run();

      } catch (const gears::Logger::Exception& e) {
         GRS_LOG_EXIT(std::format("terminating engine because of exception: {}", e.what()), e.where());

      } catch (const std::exception& e) {
         GRS_LOG_EXIT(std::format("paused on unhandled exception: {}", e.what()), std::source_location::current());
      }

   } catch (const std::ios_base::failure& e) {
      GRS_EXIT(std::format("failed to open the logFile: {}", e.what()));
   }

   return EXIT_SUCCESS;
}
