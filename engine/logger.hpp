#pragma once

#include <source_location>
#include <stdexcept>
#include <fstream>
#include <string>

namespace gears {

   class Logger {
   public:
      class Exception : public std::runtime_error {
      public:
         using baseClass = runtime_error;

         Exception(const std::string& message, const std::source_location& location = std::source_location::current())
             : _location{location}, baseClass{message.c_str()} {};

         Exception(const char* message, const std::source_location& location = std::source_location::current())
             : _location{location}, baseClass{message} {};

         std::source_location where() const { return _location; }

      private:
         std::source_location _location;
      };

      enum class Levels : uint8_t {
         Error,
         Warning,
         Trace,
         Info,
         NoLevel
      };

      Logger(const Levels level = Levels::NoLevel, const std::string& filename = "logFile.log");
      ~Logger();

      void logNoLevel(const std::string& message);
      void log(const std::string& message);
      void logTrace(const std::string& message, const std::source_location& location = std::source_location::current());
      void warn(const std::string& message, const std::source_location& location = std::source_location::current());
      void error(const std::string& message, const std::source_location& location = std::source_location::current());

      void setLevel(const Levels level) { _logLevel = level; }

   private:
      Levels _logLevel;
      std::ofstream _logFile;

      void _log(const Levels level, const std::string& message, const std::source_location& location);
   };


   inline Logger* logger = nullptr;
} // namespace gears
