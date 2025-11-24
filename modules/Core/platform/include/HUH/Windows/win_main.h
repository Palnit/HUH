#pragma once
#include <windows.h>
#include <HUH/Windows/win_instance.h>

int HUH_MAIN(int argc, char** argv);

extern "C" {
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR szCmdLine, int sw) {
    HUH::s_AppInstance = hInst;
    char test = 'a';
    char* test2 = &test;
    return HUH_MAIN(1, &test2);
}
}

#define main HUH_MAIN
