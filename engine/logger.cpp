#include <fstream>
#include <iostream>

#include "logger.hpp"

namespace gears {

   static std::ofstream logFile{"logFile.log"};

   void Logger::_log(const levels level, const std::string& message, const std::source_location& location) {
      switch (level) {
         case INFO:
            std::clog << "INFO: ";
            logFile << "INFO: ";
            break;

         case WARNING:
            std::clog << COLOR_YELLOW << "WARNING: ";
            logFile << "WARNING: ";
            break;

         case ERROR:
            std::clog << COLOR_RED << "ERROR: ";
            logFile << "ERROR: ";
            break;

         default:
            break;
      }
      std::clog << message << COLOR_DEFAULT << '\n';
      logFile << message << " [" << location.file_name() << ":" << location.line() << "]\n";
   }

   void Logger::log(const std::string& message, const std::source_location& location) {
      _log(INFO, message, location);
   }

   void Logger::warn(const std::string& message, const std::source_location& location) {
      _log(WARNING, message, location);
   }

   void Logger::error(const std::string& message, const std::source_location& location) {
      _log(ERROR, message, location);
   }

} // namespace gears