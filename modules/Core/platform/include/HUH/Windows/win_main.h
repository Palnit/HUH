#pragma once
// TODO MAKE THIS WORK ON LINUX WITHOUT SPECIAL INCLUDE
#include <windows.h>
#include <HUH/Windows/win_instance.h>

int HUH_MAIN(int argc, char** argv);

namespace HUH {
inline void WinError() {
    MessageBox(nullptr, "HUH Win init error", "Error reading windows cmd args", 0);
}

inline void WinConsoleRelease() {
    FILE* fp;

    // Just to be safe, redirect standard IO to NUL before releasing.

    // Redirect STDIN to NUL
    if (freopen_s(&fp, "NUL:", "r", stdin) != 0) {
        HUH::WinError();
    } else {
        setvbuf(stdin, nullptr, _IONBF, 0);
    }

    // Redirect STDOUT to NUL
    if (freopen_s(&fp, "NUL:", "w", stdout) != 0) {
        HUH::WinError();
    } else {
        setvbuf(stdout, nullptr, _IONBF, 0);
    }

    // Redirect STDERR to NUL
    if (freopen_s(&fp, "NUL:", "w", stderr) != 0) {
        HUH::WinError();
    } else {
        setvbuf(stderr, nullptr, _IONBF, 0);
    }

    // Detach from console
    if (!FreeConsole()) {
        HUH::WinError();
    }
}

inline void WinConsoleRedirect() {
    WinConsoleRelease();
    if (!AttachConsole(ATTACH_PARENT_PROCESS)) {
        if (!AllocConsole()) {
            HUH::WinError();
        }
    }

    FILE* fp;

    // Redirect STDIN if the console has an input handle
    if (GetStdHandle(STD_INPUT_HANDLE) != INVALID_HANDLE_VALUE) {
        if (freopen_s(&fp, "CONIN$", "r", stdin) != 0) {
            HUH::WinError();
        } else {
            setvbuf(stdin, nullptr, _IONBF, 0);
        }
    }

    // Redirect STDOUT if the console has an output handle
    if (GetStdHandle(STD_OUTPUT_HANDLE) != INVALID_HANDLE_VALUE) {
        if (freopen_s(&fp, "CONOUT$", "w", stdout) != 0) {
            HUH::WinError();
        } else {
            setvbuf(stdout, NULL, _IONBF, 0);
        }
    }

    // Redirect STDERR if the console has an error handle
    if (GetStdHandle(STD_ERROR_HANDLE) != INVALID_HANDLE_VALUE) {

        if (freopen_s(&fp, "CONOUT$", "w", stderr) != 0) {
            HUH::WinError();
        } else {
            setvbuf(stderr, NULL, _IONBF, 0);
        }
    }

    // Make C++ standard streams point to console as well.
    std::ios::sync_with_stdio(true);

    // Clear the error state for each of the C++ standard streams.
    std::wcout.clear();
    std::cout.clear();
    std::wcerr.clear();
    std::cerr.clear();
    std::wcin.clear();
    std::cin.clear();
}

}// namespace HUH

extern "C" {
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR szCmdLine, int nCmdShow) {
    HUH::g_AppInstance = hInst;
    HUH::g_CmdShow = nCmdShow;

    HUH::WinConsoleRedirect();

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

    HUH::WinConsoleRelease();
    return result;
}
}

#define main HUH_MAIN
