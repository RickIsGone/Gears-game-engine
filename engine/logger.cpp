#include <iostream>
#include <chrono>

#include "logger.hpp"

namespace gears {
   static constexpr const char* COLOR_RED = "\x1B[91m";
   static constexpr const char* COLOR_YELLOW = "\x1B[93m";
   static constexpr const char* COLOR_DEFAULT = "\x1B[0m";

   Logger::Logger(const Levels level, const std::string& filename) : logLevel{level} {
      logger = this;
      logFile.exceptions(std::ios::failbit | std::ios::badbit);
      logFile.open(filename);

#if defined(_WIN32)
#ifdef GRS_DEBUG
      logFile << "windows build running in debug mode\n";
#else
      logFile << "windows build running in release mode\n";
#endif // GRS_DEBUG
#elif defined(__linux__)
#ifdef GRS_DEBUG
      logFile << "linux build running in debug mode\n";
#else
      logFile << "linux build running in release mode\n";
#endif // GRS_DEBUG
#endif // defined(_WIN32)
   }
   Logger::~Logger() {
      logger = nullptr;
   }

   void Logger::_log(const Levels level, const std::string& message, const std::source_location& location) {
      if (logLevel >= level) {
         auto now = std::chrono::system_clock::now();
         auto in_time_t = std::chrono::system_clock::to_time_t(now);
         auto tm = *std::localtime(&in_time_t);

         double milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() % 1000;
         std::ostringstream timestamp;
         timestamp << std::put_time(&tm, "%H:%M:%S.") << std::setfill('0') << std::setw(2) << milliseconds;

         if (level != Levels::NoTrace) {
            std::clog << '[' << timestamp.str() << "] ";
            logFile << '[' << timestamp.str() << "][" << location.file_name() << ":" << location.line() << "] ";
         }
         switch (level) {
            case Levels::Info:
               std::clog << "INFO: ";
               logFile << "INFO: ";
               break;

            case Levels::Warning:
               std::clog << COLOR_YELLOW << "WARNING: ";
               logFile << "WARNING: ";
               break;

            case Levels::Error:
               std::cerr << COLOR_RED << "ERROR: ";
               logFile << "ERROR: ";
               break;

            default:
               break;
         }
         std::clog << message << COLOR_DEFAULT << '\n';
         logFile << message << std::endl; /* flushing per sicurezza */
      }
   }

   void Logger::log(const std::string& message, const std::source_location& location) {
      _log(Levels::Info, message, location);
   }

   void Logger::logNoTrace(const std::string& message, const std::source_location& location) {
      _log(Levels::NoTrace, message, location);
   }

   void Logger::warn(const std::string& message, const std::source_location& location) {
      _log(Levels::Warning, message, location);
   }

   void Logger::error(const std::string& message, const std::source_location& location) {
      _log(Levels::Error, message, location);
   }

} // namespace gears