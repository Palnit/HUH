#pragma once

#include <HUH/Math/definitions.h>
#include <HUH/definitions.h>

namespace HUH {
template<typename T>
HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE auto DegToRad(T Deg) {
    return Deg * (HUH::PI / 180.0f);
}

template<typename T>
HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE auto Clamp(T value, T min, T max) {
    return value < min ? min : (value > max ? max : value);
}

}// namespace HUH