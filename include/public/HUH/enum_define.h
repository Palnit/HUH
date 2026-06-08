#pragma once

#include <HUH/definitions.h>
#include <format>
#include <string>

#define HUH_ENUM_BIT_OPERATORS(EnumName) \
    HUH_CONSTEXPR_FORCE EnumName operator|(const EnumName lhs,const EnumName rhs) {return static_cast<EnumName>(static_cast<std::underlying_type_t<EnumName>>(lhs) | static_cast<std::underlying_type_t<EnumName>>(rhs)); } \
    HUH_CONSTEXPR_FORCE EnumName operator&(const EnumName lhs,const EnumName rhs) {return static_cast<EnumName>(static_cast<std::underlying_type_t<EnumName>>(lhs) & static_cast<std::underlying_type_t<EnumName>>(rhs)); } \
    HUH_CONSTEXPR_FORCE EnumName operator^(const EnumName lhs,const EnumName rhs) {return static_cast<EnumName>(static_cast<std::underlying_type_t<EnumName>>(lhs) ^ static_cast<std::underlying_type_t<EnumName>>(rhs)); } \
    HUH_CONSTEXPR_FORCE EnumName operator~(const EnumName Enum) {return static_cast<EnumName>(~static_cast<std::underlying_type_t<EnumName>>(Enum)); } \
    HUH_FORCE_INLINE EnumName& operator|=(EnumName& lhs,EnumName rhs) {return lhs = static_cast<EnumName>(static_cast<std::underlying_type_t<EnumName>>(lhs) | static_cast<std::underlying_type_t<EnumName>>(rhs)); } \
    HUH_FORCE_INLINE EnumName& operator&=(EnumName& lhs,EnumName rhs) {return lhs = static_cast<EnumName>(static_cast<std::underlying_type_t<EnumName>>(lhs) & static_cast<std::underlying_type_t<EnumName>>(rhs)); } \
    HUH_FORCE_INLINE EnumName& operator^=(EnumName& lhs,EnumName rhs) {return lhs = static_cast<EnumName>(static_cast<std::underlying_type_t<EnumName>>(lhs) ^ static_cast<std::underlying_type_t<EnumName>>(rhs)); }

#define HUH_ENUM_CLASS_EQUAL_OPERATOR(EnumName) \
    HUH_CONSTEXPR_FORCE bool operator==(const EnumName lhs, const std::underlying_type_t<EnumName> rhs) {return static_cast<std::underlying_type_t<EnumName>>(lhs) == rhs; } \
    HUH_CONSTEXPR_FORCE bool operator!=(const EnumName lhs, const std::underlying_type_t<EnumName> rhs) {return !(lhs == rhs); }

#define HUH_ENUM_FORMATER(EnumName) \
    template<> \
    struct std::formatter<EnumName> : std::formatter<std::string> { \
         auto format(EnumName p, format_context& ctx) const { \
            return formatter<string>::format(std::format("{}", HUH::ToString(p)), ctx); \
        } \
    };
