#pragma once

#include <HUH/concepts.h>
#include <string>
#include <type_traits>

#define HUH_ENUM_BIT_OPERATORS(EnumName) \
    HUH_CONSTEXPR_FORCE EnumName operator|(const EnumName lhs,const EnumName rhs) {return static_cast<EnumName>(static_cast<std::underlying_type_t<EnumName>>(lhs) | static_cast<std::underlying_type_t<EnumName>>(rhs)); } \
    HUH_CONSTEXPR_FORCE EnumName operator&(const EnumName lhs,const EnumName rhs) {return static_cast<EnumName>(static_cast<std::underlying_type_t<EnumName>>(lhs) & static_cast<std::underlying_type_t<EnumName>>(rhs)); } \
    HUH_CONSTEXPR_FORCE EnumName operator^(const EnumName lhs,const EnumName rhs) {return static_cast<EnumName>(static_cast<std::underlying_type_t<EnumName>>(lhs) ^ static_cast<std::underlying_type_t<EnumName>>(rhs)); } \
    HUH_CONSTEXPR_FORCE EnumName operator~(const EnumName Enum) {return static_cast<EnumName>(~static_cast<std::underlying_type_t<EnumName>>(Enum)); } \
    HUH_FORCE_INLINE EnumName& operator|=(EnumName& lhs,EnumName rhs) {return lhs = static_cast<EnumName>(static_cast<std::underlying_type_t<EnumName>>(lhs) | static_cast<std::underlying_type_t<EnumName>>(rhs)); } \
    HUH_FORCE_INLINE EnumName& operator&=(EnumName& lhs,EnumName rhs) {return lhs = static_cast<EnumName>(static_cast<std::underlying_type_t<EnumName>>(lhs) & static_cast<std::underlying_type_t<EnumName>>(rhs)); } \
    HUH_FORCE_INLINE EnumName& operator^=(EnumName& lhs,EnumName rhs) {return lhs = static_cast<EnumName>(static_cast<std::underlying_type_t<EnumName>>(lhs) ^ static_cast<std::underlying_type_t<EnumName>>(rhs)); }

namespace HUH {

template<typename EnumName>
bool CheckAllFlag(const EnumName flags, const EnumName contains) {
    return (flags & contains) == contains;
}

template<typename EnumName>
bool CheckFlag(const EnumName flags, const EnumName contains) {
    return (flags & contains) != 0;
}

#define HUH_ENUM_FORMATER(EnumName) \
template<> \
struct std::formatter<EnumName> : std::formatter<std::string> { \
    auto format(EnumName p, format_context& ctx) const { \
        return formatter<string>::format(std::format("{}", HUH::ToString(p)), ctx); \
    } \
};

}// namespace HUH
