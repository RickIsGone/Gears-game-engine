#pragma once

#include <source_location>

namespace gears {

#define COLOR_RED     "\x1B[91m"
#define COLOR_YELLOW  "\x1B[93m"
#define COLOR_DEFAULT "\x1B[0m"

   enum levels {
      INFO,
      WARNING,
      ERROR
   };

   class Logger {
   private:
      static void _log(const levels level, const std::string& message, const std::source_location& location);

   public:
      static void log(const std::string& message, const std::source_location& location = std::source_location::current());

      static void warn(const std::string& message, const std::source_location& location = std::source_location::current());

      static void error(const std::string& message, const std::source_location& location = std::source_location::current());
   };

} // namespace gears