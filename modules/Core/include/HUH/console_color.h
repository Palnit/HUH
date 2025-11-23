#pragma once

#ifdef HUH_WIN
#include <windows.h>
#endif
#include <HUH/enum_helper.h>

namespace HUH {
class ConsoleColor {
public:
    enum Color {
        Blue = 1 << 0,
        Green = 1 << 1,
        Red = 1 << 2,
        Bright = 1 << 3,
        Yellow = Red | Green,
        Magenta = Red | Blue,
        Cyan = Green | Blue,
        Reset = Blue | Green | Red
    };

    static std::string GetANSIiCode(const Color& c1) {
        if (CheckAllFlag(c1, Reset)) {
            return "\033[0m";
        }
        if (CheckAllFlag(c1, Yellow)) {
            return "\033[33m";
        }
        if (CheckAllFlag(c1, Magenta)) {
            return "\033[35m";
        }
        if (CheckAllFlag(c1, Cyan)) {
            return "\033[36m";
        }
        if (CheckAllFlag(c1, Blue)) {
            return "\033[34m";
        }
        if (CheckAllFlag(c1, Green)) {
            return "\033[32m";
        }
        if (CheckAllFlag(c1, Red)) {
            return "\033[31m";
        }
        if (CheckAllFlag(c1, Bright)) {
            return "\033[1m";
        }
        return "\033[0m";
    }

    friend std::ostream& operator<<(std::ostream& os, const ConsoleColor::Color& color) {
#ifdef HUH_WIN
        if (!SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color)) {
#endif
            os << GetANSIiCode(color);
#ifdef HUH_WIN
        }
#endif
        return os;
    }

private:
};

}// namespace HUH
