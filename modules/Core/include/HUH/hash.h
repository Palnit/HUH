#pragma once

#include <HUH/concepts.h>
#include <HUH/types.h>

#include <rapidhash.h>

namespace HUH {

template<HUH::Aritmatic T>
HUH::Uint64 Hash(const T& element, const size_t seed = 0) {
    return rapidhash_withSeed(&element, sizeof(T), seed);
}

}// namespace HUH