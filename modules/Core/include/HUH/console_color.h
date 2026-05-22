#pragma once

#ifdef HUH_WIN
#include <windows.h>
#endif
#include <HUH/enum_define.h>

#include <iostream>

namespace HUH {
class HUH_CORE_API ConsoleColor {
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

    static std::string GetANSIiCode(const Color& c1);

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
