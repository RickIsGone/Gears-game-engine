#pragma once

#include <source_location>
#include <stdexcept>
#include <fstream>
#include <string>

namespace gears {

   class Logger {
   public:
      enum class levels {
         error,
         warning,
         noTrace,
         info
      };

      Logger(const levels level = levels::info, const std::string& filename = "logFile.log");
      ~Logger();

      void log(const std::string& message, const std::source_location& location = std::source_location::current());
      void logNoTrace(const std::string& message, const std::source_location& location = std::source_location::current());
      void warn(const std::string& message, const std::source_location& location = std::source_location::current());
      void error(const std::string& message, const std::source_location& location = std::source_location::current());

      void setLevel(const levels level) { logLevel = level; }

      class Exception : public std::runtime_error {
      private:
         std::source_location loc;

      public:
         using baseClass = runtime_error;

         Exception(const std::string& message, const std::source_location& location = std::source_location::current())
             : loc{location}, baseClass{message.c_str()} {};

         Exception(const char* message, const std::source_location& location = std::source_location::current())
             : loc{location}, baseClass{message} {};

         std::source_location where() const { return loc; }
      };

   private:
      levels logLevel;
      std::ofstream logFile;

      void _log(const levels level, const std::string& message, const std::source_location& location);
   };


   inline Logger* logger = nullptr;
} // namespace gears
