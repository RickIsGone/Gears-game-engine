#include "Application.hpp"
#include "macro.hpp"
#include "logger.hpp"

/* creato logger globale in logger.hpp */
int main(int argc, char** argv) {
   try {
      gears::Logger logger;

      try {
         gears::logger->log("initializing engine");
         std::unique_ptr<gears::Application> app = std::make_unique<gears::Application>(); /* sopra 1KB allocare heap */

         gears::logger->log("starting engine");
         app->run();

      } catch (const gears::Logger::Exception& e) {
         GRS_LOG_EXIT("terminating engine because of exception: " + (std::string)e.what(), e.where());

      } catch (const std::exception& e) {
         GRS_LOG_EXIT("paused on unhandled exception: " + (std::string)e.what(), std::source_location::current());
      }
      gears::logger->log("engine successfully terminated");

   } catch (const std::ios_base::failure& e) {
      GRS_EXIT("failed to open the logFile: " + (std::string)e.what());
   }

   return EXIT_SUCCESS;
}
