#pragma once

#include <HUH/concepts.h>
#include <HUH/types.h>

#include <rapidhash.h>

namespace HUH {

template<HUH::Aritmatic T>
HUH_NODISCARD HUH_CONSTEXPR_FORCE HUH::Uint64 Hash(const T& element, const size_t seed = 0) {
    return rapidhash_withSeed(&element, sizeof(T), seed);
}

HUH_NODISCARD HUH_CONSTEXPR_FORCE HUH::Uint64 Hash(const std::string& element, const size_t seed = 0) {
    return rapidhash_withSeed(element.data(), element.size(), seed);
}

}// namespace HUH