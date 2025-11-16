#pragma once

#ifdef HUH_WIN
#include <windows.h>
#endif

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
    friend Color operator|(const Color& c1, const Color& c2) {
        return static_cast<Color>(static_cast<int>(c1) | static_cast<int>(c2));
    }

    friend Color operator&(const Color& c1, const Color& c2) {
        return static_cast<Color>(static_cast<int>(c1) & static_cast<int>(c2));
    }

    static std::string GetANSIiCode(const Color& c1) {
        if (c1 & Red && c1 & Green && c1 & Blue) {
            return "\033[0m";
        }
        if (c1 & Red && c1 & Green) {
            return "\033[33m";
        }
        if (c1 & Red && c1 & Blue) {
            return "\033[35m";
        }
        if (c1 & Blue && c1 & Green) {
            return "\033[36m";
        }
        if (c1 & Blue) {
            return "\033[34m";
        }
        if (c1 & Green) {
            return "\033[32m";
        }
        if (c1 & Red) {
            return "\033[31m";
        }
        if (c1 & Bright) {
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
