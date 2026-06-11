#pragma once

#include <HUH/Math/fwd.h>
#include <HUH/definitions.h>
#include <cmath>

#if HUH_USE_SIMD
#include <HUH/Simd/register.h>
#include <HUH/Simd/simd_functions.h>
#endif

namespace HUH {

template<typename T, typename T2, std::size_t N>
HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE std::common_type_t<T, T2> Dot(const HUH::Vector<T, N>& lhs,
                                                                      const HUH::Vector<T2, N>& rhs) noexcept {
    std::common_type_t<T, T2> result{};
    for (std::size_t i = 0; i < N; ++i) {
        result = std::fma(lhs[i], rhs[i], result);
    }
    return result;
}

template<typename T>
HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE T Dot(const HUH::Vector<T, 4>& lhs, const HUH::Vector<T, 4>& rhs) noexcept {
    return lhs.data[0] * rhs.data[0] + lhs.data[1] * rhs.data[1] + lhs.data[2] * rhs.data[2]
        + lhs.data[3] * rhs.data[3];
}

template<typename T>
HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE T Dot(const HUH::Vector<T, 3>& lhs, const HUH::Vector<T, 3>& rhs) noexcept {
    return lhs.data[0] * rhs.data[0] + lhs.data[1] * rhs.data[1] + lhs.data[2] * rhs.data[2];
}

template<typename T>
HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE T Dot(const HUH::Vector<T, 2>& lhs, const HUH::Vector<T, 2>& rhs) noexcept {
    return lhs.data[0] * rhs.data[0] + lhs.data[1] * rhs.data[1];
}

#if HUH_USE_SIMD
HUH_FORCE_INLINE float Dot(const HUH::Vector<float, 4>& lhs, const HUH::Vector<float, 4>& rhs) noexcept;

HUH_FORCE_INLINE float Dot(const HUH::Vector<float, 3>& lhs, const HUH::Vector<float, 3>& rhs) noexcept;

// HUH_FORCE_INLINE float Dot(const HUH::Vector<float, 2>& lhs,
//                            const HUH::Vector<float, 2>& rhs) noexcept;
#endif

template<typename T, std::size_t N>
HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE void Normalize(HUH::Vector<T, N>& vec) noexcept {
    T tmp = HUH::Dot(vec, vec);
    tmp = std::sqrt(tmp);
    vec /= tmp;
}

#if HUH_USE_SIMD
HUH_FORCE_INLINE void Normalize(HUH::Vector<float, 4>& vec) noexcept;
HUH_FORCE_INLINE void Normalize(HUH::Vector<float, 3>& vec) noexcept;
// HUH_FORCE_INLINE void Normalize(HUH::Vector<float, 2>& vec) noexcept;
#endif

// TODO simd version ?
template<typename T, typename T2>
HUH_FORCE_INLINE void Cross(const Vector<T, 3>& lhs,
                            const Vector<T2, 3>& rhs,
                            HUH::Vector<std::common_type_t<T, T2>, 3>& result) noexcept {
    result[0] = lhs[1] * rhs[2] - lhs[2] * rhs[1];
    result[1] = lhs[2] * rhs[0] - lhs[0] * rhs[2];
    result[2] = lhs[0] * rhs[1] - lhs[1] * rhs[0];
}

template<typename T, size_t N>
HUH_FORCE_INLINE void Norm(const Vector<T, N>& lhs) noexcept {
    return HUH::Dot(lhs, lhs);
}

}// namespace HUH
