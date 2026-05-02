#pragma once

#include <HUH/concepts.h>
#include <HUH/types.h>
#include <format>
#include <string>
#include <type_traits>

namespace HUH {

template<typename EnumName>
bool CheckAllFlag(const EnumName flags, const EnumName contains) {
    return (flags & contains) == contains;
}

template<typename EnumName>
bool CheckFlag(const EnumName flags, const EnumName contains) {
    return (flags & contains) != 0;
}

}// namespace HUH
