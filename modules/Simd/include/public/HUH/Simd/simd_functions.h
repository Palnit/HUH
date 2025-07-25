#pragma once

#include "register.h"
#include "shuffle.h"

namespace HUH::Simd {

// https://stackoverflow.com/questions/6996764/fastest-way-to-do-horizontal-sse-vector-sum-or-other-reduction/35270026#35270026
inline float HSum(const Register<float, 4>& reg) noexcept {
    Register<float, 4> shuffle = ShuffleRegister<1, 1, 3, 3>(reg);
    Register<float, 4> sum = reg + shuffle;
    shuffle = HUH::Simd::ShuffleRegister<2, 3, 2, 3>(shuffle, sum);
    sum = _mm_add_ss(sum, shuffle);
    return _mm_cvtss_f32(sum);
}

inline float InvSqrt(float x) noexcept {
    const __m128 One = _mm_set_ss(1.0f);
    const __m128 Y0 = _mm_set_ss(x);
    const __m128 X0 = _mm_sqrt_ss(Y0);
    const __m128 R0 = _mm_div_ss(One, X0);
    float temp;
    _mm_store_ss(&temp, R0);
    return temp;
}

}// namespace HUH::Simd
