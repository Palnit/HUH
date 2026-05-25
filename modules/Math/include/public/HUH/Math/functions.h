#pragma once

#include <HUH/Math/definitions.h>
#include <HUH/definitions.h>

namespace HUH {
template<typename T>
HUH_CONSTEXPR_FORCE auto DegToRad(T Deg) {
    return Deg * (HUH::PI / 180.0f);
}

}// namespace HUH