#include "Application.hpp"
#include "macro.hpp"
#include "logger.hpp"

int main(int argc, char** argv) {
   try {
      gears::Logger::check();

      gears::Logger::log("initializing engine");
      gears::Application app{};
      // std::unique_ptr<gears::Application> app = std::make_unique<gears::Application>();  se supera i 0.8MB (1MB max sullo stack)

      gears::Logger::log("starting engine");
      app.run();

   } catch (const gears::Logger::loggerException& e) {
      gears::Logger::error("terminating engine because of exception: " + std::string(e.what()), e.where());
      GRS_PAUSE;
      return EXIT_FAILURE;
   } catch (...) {
      GRS_EXIT("failed to open the logFile");
   }

   gears::Logger::log("engine successfully terminated");
   return EXIT_SUCCESS;
}
