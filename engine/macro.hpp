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

#define GRS_EXIT(x) \
   std::cerr << x;  \
   GRS_PAUSE;       \
   exit(EXIT_FAILURE)

#else

#define GRS_PAUSE
#define GRS_EXIT(x) exit(EXIT_FAILURE)

#endif // GRS_DEBUG

#ifndef LOGGER_IMPORT
import logger;
#endif // ifndef LOGGER_IMPORT
#define GRS_LOG_EXIT(x, y)     \
   gears::logger->error(x, y); \
   GRS_PAUSE;                  \
   exit(EXIT_FAILURE)
