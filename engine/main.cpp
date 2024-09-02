#include "Application.hpp"
#include "macro.hpp"
#include "logger.hpp"

int main(int argc, char** argv) {
   try {
      gears::Logger::check();
   } catch (...) {
      GRS_EXIT("failed to open the logFile");
   }

   try {
      gears::Logger::log("initializing engine");
      gears::Application app{};
      // std::unique_ptr<gears::Application> app = std::make_unique<gears::Application>();  se supera i 0.8MB (1MB max sullo stack)

      gears::Logger::log("starting engine");
      app.run();

   } catch (const gears::Logger::loggerException& e) {
      GRS_LOG_EXIT("terminating engine because of exception: " + std::string(e.what()), e.where());

   } catch (const std::exception& e) {
      GRS_LOG_EXIT("paused on unhandled exception: " + std::string(e.what()), std::source_location::current());
   }

   gears::Logger::log("engine successfully terminated");
   return EXIT_SUCCESS;
}
