#include <format>
#include <iostream>
#include <chrono>

#include "logger.hpp"

namespace gears {
   static constexpr const char* COLOR_RED = "\x1B[91m";
   static constexpr const char* COLOR_YELLOW = "\x1B[93m";
   static constexpr const char* COLOR_DEFAULT = "\x1B[0m";

   Logger::Logger(const Levels level, const std::string& filename) : _logLevel{level} {
      logger = this;
      _logFile.exceptions(std::ios::failbit | std::ios::badbit);
      _logFile.open(filename);

#if defined(_WIN32)
#ifdef GRS_DEBUG
      _logFile << "windows build running in debug mode\n";
#else
      _logFile << "windows build running in release mode\n";
#endif // GRS_DEBUG
#elif defined(__linux__)
#ifdef GRS_DEBUG
      _logFile << "linux build running in debug mode\n";
#else
      _logFile << "linux build running in release mode\n";
#endif // GRS_DEBUG
#endif // defined(_WIN32)
   }
   Logger::~Logger() {
      logger = nullptr;
   }

   void Logger::_log(const Levels level, const std::string& message, const std::source_location& location) {
      if (_logLevel >= level) {
         auto now = std::chrono::system_clock::now();
         auto in_time_t = std::chrono::system_clock::to_time_t(now);
         auto tm = *std::localtime(&in_time_t);

         double milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() % 1000;
         std::ostringstream timestamp;
         timestamp << std::put_time(&tm, "%H:%M:%S.") << std::setfill('0') << std::setw(2) << milliseconds;

         switch (level) {
            case Levels::Trace:
               std::clog << std::format("[{}]", timestamp.str());
               _logFile << std::format("[{}][{}:{}]", timestamp.str(), location.file_name(), location.line());
               // niente break perche' deve fare anche le robe del livello Info
            case Levels::Info:
               std::clog << "[INFO] ";
               _logFile << "[INFO] ";
               break;

            case Levels::Warning:
               std::clog << std::format("{}[{}][WARNING] ", COLOR_YELLOW, timestamp.str());
               _logFile << std::format("[{}][{}:{}][WARNING] ", timestamp.str(), location.file_name(), location.line());
               break;

            case Levels::Error:
               std::clog << std::format("{}[{}][ERROR] ", COLOR_RED, timestamp.str());
               _logFile << std::format("[{}][{}:{}][ERROR] ", timestamp.str(), location.file_name(), location.line());
               break;

            default: // Levels::NoLevel
               break;
         }
         std::clog << std::format("{}{}\n", message, COLOR_DEFAULT);
         _logFile << message << std::endl; /* flushing per sicurezza */
      }
   }

   void Logger::logNoLevel(const std::string& message) {
      _log(Levels::NoLevel, message, std::source_location::current());
   }

   void Logger::log(const std::string& message) {
      _log(Levels::Info, message, std::source_location::current());
   }

   void Logger::logTrace(const std::string& message, const std::source_location& location) {
      _log(Levels::Trace, message, location);
   }

   void Logger::warn(const std::string& message, const std::source_location& location) {
      _log(Levels::Warning, message, location);
   }

   void Logger::error(const std::string& message, const std::source_location& location) {
      _log(Levels::Error, message, location);
   }

} // namespace gears