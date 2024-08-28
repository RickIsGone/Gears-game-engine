#pragma once

#ifdef GRS_DEBUG
   #include <iostream>

   #if defined(_WIN32)
      #define GRS_PAUSE system("pause")

   #elif defined(__linux__) || defined(_LINUX_)
      #define GRS_PAUSE                             \
         std::cout << "press enter to continue..."; \
         std::cin.get()

   #endif

   #define GRS_EXIT(x) \
      std::cerr << x;  \
      GRS_PAUSE;       \
      exit(EXIT_FAILURE)

#else

   #define GRS_PAUSE
   #define GRS_EXIT(x)

   #if defined(_WIN32)
      #include <Windows.h>
int main(int, char**);
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
   (void)hInstance;
   (void)hPrevInstance;
   (void)lpCmdLine;
   (void)nShowCmd;

   return main(__argc, __argv);
}
   #endif

#endif


#define GRS_LOG_EXIT(x, y)     \
   gears::Logger::error(x, y); \
   GRS_PAUSE;                  \
   exit(EXIT_FAILURE)


// #if defined(_WIN32)

// #elif defined(__linux__) || defined(_LINUX_)

// #endif