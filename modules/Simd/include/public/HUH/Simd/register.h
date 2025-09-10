#pragma once

#include <HUH/definitions.h>
#include <type_traits>

namespace HUH::Simd {
template<typename T, std::size_t N>
class Register {
//    static_assert(false, "Template type is not allowed"); // msvc not working here ?
};
}// namespace HUH::Simd

#include "register/register_float4.h"
