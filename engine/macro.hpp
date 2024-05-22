#pragma once



#ifndef NDEBUG

    #ifdef _WIN32
        #define GEARS_DEBUG_WAIT system("pause")
    #else
        #define GEARS_DEBUG_WAIT system("xterm -e 'read -p \"Press Enter to continue...\"'")
    #endif

    #define GEARS_ASSERT(expression) (void)( (!!(expression)) || (_wassert(_CRT_WIDE(#expression), _CRT_WIDE(__FILE__), (unsigned)(__LINE__)), 0) )

#else

    #define GEARS_DEBUG_WAIT //
    #define GEARS_ASSERT //

    #ifdef WIN32
        #include <Windows.h>
        int main(int, char**);
        int __stdcall WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd){
            (void) hInstance;
            (void) hPrevInstance;
            (void) lpCmdLine;
            (void) nShowCmd;

            return main(__argc, __argv);
        }
    #endif

#endif