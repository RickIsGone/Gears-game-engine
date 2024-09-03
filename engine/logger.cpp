#include <fstream>
#include <iostream>
#include <chrono>

#include "logger.hpp"

namespace gears::Logger {

   static std::ofstream logFile;

   static void _log(const levels level, const std::string& message, const std::source_location& location) {
      auto now = std::chrono::system_clock::now();
      auto in_time_t = std::chrono::system_clock::to_time_t(now);
      auto tm = *std::localtime(&in_time_t);

      double milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() % 1000;
      std::ostringstream timestamp;
      timestamp << std::put_time(&tm, "%H:%M:%S.") << std::setfill('0') << std::setw(2) << milliseconds;

      if (level != levels::L_NOTRACE) {
         std::clog << '[' << timestamp.str() << "] ";
         logFile << '[' << timestamp.str() << "][" << location.file_name() << ":" << location.line() << "] ";
      }
      switch (level) {
         case levels::L_INFO:
            std::clog << "INFO: ";
            logFile << "INFO: ";
            break;

         case levels::L_WARNING:
            std::clog << COLOR_YELLOW << "WARNING: ";
            logFile << "WARNING: ";
            break;

         case levels::L_ERROR:
            std::cerr << COLOR_RED << "ERROR: ";
            logFile << "ERROR: ";
            break;

         default:
            break;
      }
      std::clog << message << COLOR_DEFAULT << '\n';
      logFile << message << std::endl; // flushing per sicurezza
   }

   void log(const std::string& message, const std::source_location& location) {
      _log(levels::L_INFO, message, location);
   }

   void logNoTrace(const std::string& message, const std::source_location& location) {
      _log(levels::L_NOTRACE, message, location);
   }

   void warn(const std::string& message, const std::source_location& location) {
      _log(levels::L_WARNING, message, location);
   }

   void error(const std::string& message, const std::source_location& location) {
      _log(levels::L_ERROR, message, location);
   }

   void check() {
      logFile.exceptions(std::ios::failbit);
      logFile.open("logFile.log");

#if defined(_WIN32)
   #ifdef GRS_DEBUG
      gears::Logger::logNoTrace("windows build running in debug mode");
   #else
      gears::Logger::logNoTrace("windows build running in release mode");
   #endif
#elif defined(__linux__)
   #ifdef GRS_DEBUG
      gears::Logger::logNoTrace("linux build running in debug mode");
   #else
      gears::Logger::logNoTrace("linux build running in release mode");
   #endif
#endif
   }

} // namespace gears::Logger