#pragma once

#if defined(_WIN32)
#include <Windows.h>
int main(int, char**);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
   (void)hInstance;
   (void)hPrevInstance;
   (void)lpCmdLine;
   (void)nShowCmd;

   return main(__argc, __argv);
}
#endif // defined(_WIN32)