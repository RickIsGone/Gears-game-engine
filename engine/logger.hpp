#pragma once

#include <source_location>
#include <string>

namespace gears::Logger {
   class loggerException : public std::exception {
   private:
      std::source_location loc;

   public:
      using baseClass = exception;

      loggerException(const std::string& message, const std::source_location& location = std::source_location::current())
          : loc{location}, baseClass{message.c_str()} {};

      loggerException(const char* message, const std::source_location& location = std::source_location::current())
          : loc{location}, baseClass{message} {};

      std::source_location where() const { return loc; }
   };

#define COLOR_RED     "\x1B[91m"
#define COLOR_YELLOW  "\x1B[93m"
#define COLOR_DEFAULT "\x1B[0m"

   enum class levels {
      L_INFO,
      L_NOTRACE,
      L_WARNING,
      L_ERROR
   };


   void log(const std::string& message, const std::source_location& location = std::source_location::current());

   void logNoTrace(const std::string& message, const std::source_location& location = std::source_location::current());

   void warn(const std::string& message, const std::source_location& location = std::source_location::current());

   void error(const std::string& message, const std::source_location& location = std::source_location::current());

   void check();

} // namespace gears::Logger