#pragma once

#include <HUH/Simd/register.h>
#include <HUH/Simd/shuffle.h>
#include <HUH/definitions.h>

namespace HUH::Simd {

// https://stackoverflow.com/questions/6996764/fastest-way-to-do-horizontal-sse-vector-sum-or-other-reduction/35270026#35270026
HUH_FORCE_INLINE float HSum(const Register<float, 4>& reg) noexcept {
    Register<float, 4> shuffle = ShuffleRegister<1, 1, 3, 3>(reg);
    Register<float, 4> sum = reg + shuffle;
    shuffle = HUH::Simd::ShuffleRegister<2, 3, 2, 3>(shuffle, sum);
    sum = _mm_add_ss(sum, shuffle);
    return _mm_cvtss_f32(sum);
}

HUH_FORCE_INLINE float InvSqrt(float x) noexcept {
    const __m128 One = _mm_set_ss(1.0f);
    __m128 Y0 = _mm_set_ss(x);
    Y0 = _mm_sqrt_ss(Y0);
    Y0 = _mm_div_ss(One, Y0);
    float temp;
    _mm_store_ss(&temp, Y0);
    return temp;
}

HUH_FORCE_INLINE Register<float, 4> FMA(const Register<float, 4>& a,
                                        const Register<float, 4>& b,
                                        const Register<float, 4>& c) noexcept {
#ifdef HUH_FMA_SUPPORT
    return _mm_fmadd_ps(a, b, c);
#else
    return (a * b) + c;
#endif
}

}// namespace HUH::Simd
