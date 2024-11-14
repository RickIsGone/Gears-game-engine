#pragma once

#ifdef GRS_DEBUG
#include <iostream>

#if defined(_WIN32)
#define GRS_PAUSE system("pause")
#elif defined(__linux__)
#define GRS_PAUSE                             \
   std::cout << "press enter to continue..."; \
   std::cin.get()

#endif // defined(_WIN32)

#define GRS_EXIT(err)        \
   std::cerr << err << '\n'; \
   GRS_PAUSE;                \
   exit(EXIT_FAILURE)

#else

#define GRS_PAUSE
#define GRS_EXIT(x) exit(EXIT_FAILURE)

#endif // defined(GRS_DEBUG)

#ifndef IMPORT_LOGGER
import engine.logger;
#endif // !defined(LOGGER_IMPORT)
#define GRS_LOG_EXIT(loc, err, ...)             \
   gears::logger->error(loc, err, __VA_ARGS__); \
   GRS_PAUSE;                                   \
   exit(EXIT_FAILURE)


#define GRS_ASSERT(expr, err) \
   if (!(expr)) { GRS_EXIT(err); }

#define GRS_LOG_ASSERT(expr, err, ...)        \
   if (!(expr)) {                             \
      gears::logger->error(err, __VA_ARGS__); \
      GRS_PAUSE;                              \
      exit(EXIT_FAILURE);                     \
   }