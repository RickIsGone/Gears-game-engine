#pragma once



#define GEARS_ASSERT assert


#ifndef NDEBUG

    #ifdef _WIN32
        #define GEARS_DEBUG_WAIT system("pause")
    #else
        #define GEARS_DEBUG_WAIT system("xterm -e 'read -p \"Press Enter to continue...\"'")
    #endif

#else

    #define GEARS_DEBUG_WAIT //

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