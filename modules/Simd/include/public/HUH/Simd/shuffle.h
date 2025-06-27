#pragma once

#define HUH_SHUFFLE_MASK(X, Y, Z, W)             \
    ((X) | ((Y) << 2) | ((Z) << 4) | ((W) << 6))

#ifdef HUH_SSE3_SUPPORT
#include <pmmintrin.h>
#endif

#ifdef HUH_AVX2_SUPPORT
#include <avx2intrin.h>
#endif

#include "register.h"

namespace HUH::Simd {
/** ---------------- **/
/** float 4 shuffles **/
/** ---------------- **/

template<int X, int Y, int Z, int W>
inline Register<float, 4> ShuffleRegister(
    const Register<float, 4>& reg) noexcept {
    return _mm_shuffle_ps(reg, reg, HUH_SHUFFLE_MASK(X, Y, Z, W));
}

template<>
inline Register<float, 4> ShuffleRegister<0, 1, 2, 3>(
    const Register<float, 4>& reg) noexcept {
    return reg;
}

template<>
inline Register<float, 4> ShuffleRegister<0, 1, 0, 1>(
    const Register<float, 4>& reg) noexcept {
    return _mm_movelh_ps(reg, reg);
}

template<>
inline Register<float, 4> ShuffleRegister<2, 3, 2, 3>(
    const Register<float, 4>& reg) noexcept {
    return _mm_movehl_ps(reg, reg);
}

template<>
inline Register<float, 4> ShuffleRegister<0, 0, 1, 1>(
    const Register<float, 4>& reg) noexcept {
    return _mm_unpacklo_ps(reg, reg);
}

template<>
inline Register<float, 4> ShuffleRegister<2, 2, 3, 3>(
    const Register<float, 4>& reg) noexcept {
    return _mm_unpackhi_ps(reg, reg);
}

#ifdef HUH_SSE3_SUPPORT
template<>
inline Register<float, 4> ShuffleRegister<0, 0, 2, 2>(
    const Register<float, 4>& reg) noexcept {
    return _mm_moveldup_ps(reg);
}

template<>
inline Register<float, 4> ShuffleRegister<1, 1, 3, 3>(
    const Register<float, 4>& reg) noexcept {
    return _mm_movehdup_ps(reg);
}
#endif

#ifdef HUH_AVX2_SUPPORT
template<>
inline Register<float, 4> ShuffleRegister<0, 0, 0, 0>(
    const Register<float, 4>& reg) noexcept {
    return _mm_broadcastss_ps(reg);
}

template<int X, int Y, int Z, int W>
inline Register<float, 4> ShuffleRegister(
    const Register<float, 4>& reg,
    const Register<float, 4>& reg2) noexcept {
    return _mm_shuffle_ps(reg, reg2, HUH_SHUFFLE_MASK(X, Y, Z, W));
}

template<>
inline Register<float, 4> ShuffleRegister<0, 1, 0, 1>(
    const Register<float, 4>& reg,
    const Register<float, 4>& reg2) noexcept {
    return _mm_movelh_ps(reg, reg2);
}

template<>
inline Register<float, 4> ShuffleRegister<2, 3, 2, 3>(
    const Register<float, 4>& reg,
    const Register<float, 4>& reg2) noexcept {
    return _mm_movehl_ps(reg, reg2);
}

template<>
inline Register<float, 4> ShuffleRegister<0, 0, 1, 1>(
    const Register<float, 4>& reg,
    const Register<float, 4>& reg2) noexcept {
    return _mm_unpacklo_ps(reg, reg2);
}

template<>
inline Register<float, 4> ShuffleRegister<2, 2, 3, 3>(
    const Register<float, 4>& reg,
    const Register<float, 4>& reg2) noexcept {
    return _mm_unpackhi_ps(reg, reg2);
}
}
#endif
