module;

#include <source_location>
#include <fstream>
#include <string>
#include <format>
#include <iostream>
#include <chrono>
#include <string_view>

#define LOGGER_IMPORT
#include "macro.hpp"

export module engine.logger;

namespace gears {

   export class Logger {
   public:
      struct Message {
         Message(const char* msg, const std::source_location& loc = std::source_location::current()) : message{msg}, location{loc} {}
         Message(const std::string& msg, const std::source_location& loc = std::source_location::current()) : message{msg}, location{loc} {}

         std::string_view message;
         std::source_location location;
      };
      class Exception : public std::runtime_error {
      public:
         using baseClass = runtime_error;

         template <typename... Args>
         Exception(const Message& m, Args&&... args)
             : _location{m.location}, baseClass{std::vformat(m.message, std::make_format_args(args...))} {}

         template <typename... Args>
         Exception(const std::source_location& location, const char* message, Args&&... args)
             : _location{location}, baseClass{std::vformat(message, std::make_format_args(args...))} {}

         std::source_location where() const { return _location; }

      private:
         std::source_location _location;
      };

      enum class Levels : uint8_t {
         Disabled,
         Error,
         Warning,
         Trace,
         Info,
         NoLevel
      };

      Logger(const Levels level = Levels::NoLevel, const std::string& filename = "logFile.log");
      ~Logger();

      template <typename... Args>
      void logNoLevel(std::string_view message, Args&&... args);

      template <typename... Args>
      void log(std::string_view message, Args&&... args);

      template <typename... Args>
      void logTrace(const Message& m, Args&&... args);
      template <typename... Args>
      void logTrace(const std::source_location& location, std::string_view message, Args&&... args);

      template <typename... Args>
      void warn(const Message& m, Args&&... args);
      template <typename... Args>
      void warn(const std::source_location& location, std::string_view message, Args&&... args);

      template <typename... Args>
      void error(const Message& m, Args&&... args);
      template <typename... Args>
      void error(const std::source_location& location, std::string_view message, Args&&... args);

      void setLevel(const Levels level) { _logLevel = level; }

   private:
      Levels _logLevel;
      std::ofstream _logFile;

      void _log(const Levels level, std::string_view message, const std::source_location& location);
   };


   export inline Logger* logger = nullptr;

   // ========================================== implementation ==========================================

   constexpr const char* COLOR_RED = "\x1B[91m";
   constexpr const char* COLOR_YELLOW = "\x1B[93m";
   constexpr const char* COLOR_DEFAULT = "\x1B[0m";

   Logger::Logger(const Levels level, const std::string& filename) : _logLevel{level} {
      try {
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
      } catch (const std::ios_base::failure& e) {
         GRS_EXIT(std::format("failed to open the logFile: {}", e.what()));
      }
   }
   Logger::~Logger() {
      logger = nullptr;
   }


   void Logger::_log(const Levels level, std::string_view message, const std::source_location& location) {
      GRS_ASSERT(logger, "the logger was nullptr");
      if (_logLevel >= level) {
         auto now = std::chrono::system_clock::now();
         auto in_time_t = std::chrono::system_clock::to_time_t(now);
         auto tm = *std::localtime(&in_time_t);

         double milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() % 1000;
         std::ostringstream timestamp;
         timestamp << std::put_time(&tm, "%H:%M:%S.") << std::setfill('0') << std::setw(2) << milliseconds;

#if defined(_WIN32)
#define separator '\\'
#elif defined(__linux__)
#define separator '/'
#endif // defined(_WIN32)
         std::string fileName = location.file_name();
         fileName = fileName.substr(fileName.find_last_of(separator) + 1);
#undef separator
         switch (level) {
            case Levels::Trace:
               std::clog << std::format("[{}]", timestamp.str());
               _logFile << std::format("[{}][{}:{}]", timestamp.str(), fileName, location.line());
               // niente break perche' deve fare anche le robe del livello Info
            case Levels::Info:
               std::clog << "[INFO] ";
               _logFile << "[INFO] ";
               break;

            case Levels::Warning:
               std::clog << std::format("{}[{}][WARNING] ", COLOR_YELLOW, timestamp.str());
               _logFile << std::format("[{}][{}:{}][WARNING] ", timestamp.str(), fileName, location.line());
               break;

            case Levels::Error:
               std::clog << std::format("{}[{}][ERROR] ", COLOR_RED, timestamp.str());
               _logFile << std::format("[{}][{}:{}][ERROR] ", timestamp.str(), fileName, location.line());
               break;

            default: // Levels::NoLevel
               break;
         }
         std::clog << std::format("{}{}\n", message, COLOR_DEFAULT);
         _logFile << message << std::endl; /* flushing per sicurezza */
      }
   }

   template <typename... Args>
   void Logger::logNoLevel(std::string_view message, Args&&... args) {
      _log(Levels::NoLevel, std::vformat(message, std::make_format_args(args...)), std::source_location::current());
   }

   template <typename... Args>
   void Logger::log(std::string_view message, Args&&... args) {
      _log(Levels::Info, std::vformat(message, std::make_format_args(args...)), std::source_location::current());
   }

   template <typename... Args>
   void Logger::logTrace(const Message& m, Args&&... args) {
      _log(Levels::Trace, std::vformat(m.message, std::make_format_args(args...)), m.location);
   }
   template <typename... Args>
   void Logger::logTrace(const std::source_location& location, std::string_view message, Args&&... args) {
      _log(Levels::Trace, std::vformat(message, std::make_format_args(args...)), location);
   }

   template <typename... Args>
   void Logger::warn(const Message& m, Args&&... args) {
      _log(Levels::Warning, std::vformat(m.message, std::make_format_args(args...)), m.location);
   }
   template <typename... Args>
   void Logger::warn(const std::source_location& location, std::string_view message, Args&&... args) {
      _log(Levels::Warning, std::vformat(message, std::make_format_args(args...)), location);
   }

   template <typename... Args>
   void Logger::error(const Message& m, Args&&... args) {
      _log(Levels::Error, std::vformat(m.message, std::make_format_args(args...)), m.location);
   }
   template <typename... Args>
   void Logger::error(const std::source_location& location, std::string_view message, Args&&... args) {
      _log(Levels::Error, std::vformat(message, std::make_format_args(args...)), location);
   }
} // namespace gears
