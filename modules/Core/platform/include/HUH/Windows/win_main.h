#pragma once
#include <windows.h>
#include <HUH/Windows/win_instance.h>

int HUH_MAIN(int argc, char** argv);

namespace HUH {
inline void WinError() {
    MessageBox(nullptr, "HUH Win init error", "Error reading windows cmd args", 0);
}

}// namespace HUH

extern "C" {
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR szCmdLine, int nCmdShow) {
    HUH::g_AppInstance = hInst;
    HUH::g_CmdShow = nCmdShow;

    LPWSTR cmdLineFull = GetCommandLineW();
    const int argSize = WideCharToMultiByte(CP_UTF8, 0, cmdLineFull, -1, nullptr, 0, nullptr, nullptr);
    if (!argSize) {
        HUH::WinError();
        return -1;
    }

    int argc;
    LPWSTR* argvW = CommandLineToArgvW(cmdLineFull, &argc);
    if (!argvW || argc < 0) {
        LocalFree(argvW);
        HUH::WinError();
        return -2;
    }

    char** argv = static_cast<char**>(std::malloc((argc + 1) * sizeof(char*) + argSize));
    if (!argv) {
        HUH::WinError();
        std::free(argv);
        LocalFree(argvW);
        return -3;
    }

    char* argData = reinterpret_cast<char*>(argv) + (argc + 1) * sizeof(*argv);
    int dataBytes = 0;

    for (int i = 0; i < argc; ++i) {
        const int bytesWritten =
            WideCharToMultiByte(CP_UTF8, 0, argvW[i], -1, argData + dataBytes, argSize - dataBytes, nullptr, nullptr);
        if (!bytesWritten) {
            HUH::WinError();
            std::free(argv);
            LocalFree(argvW);
            return -4;
        }
        argv[i] = argData + dataBytes;
        dataBytes += bytesWritten;
    }
    argv[argc] = nullptr;

    const auto result = HUH_MAIN(argc, argv);
    LocalFree(argvW);
    std::free(argv);
    return result;
}
}

#define main HUH_MAIN
