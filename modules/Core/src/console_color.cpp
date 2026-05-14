#include <HUH/console_color.h>

#include <HUH/enum_helper.h>

namespace HUH {
std::string ConsoleColor::GetANSIiCode(const Color& c1) {
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
}// namespace HUH