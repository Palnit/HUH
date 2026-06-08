#pragma once

#include <HUH/Math/fwd.h>
#include <HUH/definitions.h>
#include <cmath>
#include <iostream>

namespace HUH {

template<typename T, typename T2, std::size_t R, std::size_t C, std::size_t C2>
HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE void MatrixMultiply(const Matrix<T, R, C>& lhs,
                                                            const Matrix<T2, C, C2>& rhs,
                                                            Matrix<std::common_type_t<T, T2>, R, C2>& result) {
    for (std::size_t i = 0; i < R; i++) {
        for (std::size_t k = 0; k < C; k++) {
            for (std::size_t j = 0; j < C2; j++) {
                result.data[i][j] = std::fma(lhs.data[i][k], rhs.data[k][j], result.data[i][j]);
            }
        }
    }
}

template<typename T, typename T2>
HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE void MatrixMultiply(const Matrix4x4<T>& lhs,
                                                            const Matrix4x4<T2>& rhs,
                                                            Matrix4x4<std::common_type_t<T, T2>>& result);

#if HUH_USE_SIMD
HUH_FORCE_INLINE void MatrixMultiply(const Matrix4x4<float>& lhs,
                                     const Matrix4x4<float>& rhs,
                                     Matrix4x4<float>& result);
#endif

template<typename T, size_t R, size_t C>
HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Matrix<T, C, R> Transpose(const Matrix<T, R, C>& lhs) {
    std::cout << "Transpose() R C" << std::endl;
    return Matrix<T, C, R>();
}

template<typename T, size_t N>
HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Matrix<T, N, N> Transpose(const Matrix<T, N, N>& lhs) {
    std::cout << "Transpose() N" << std::endl;
    return Matrix<T, N, N>(lhs.data);
}

template<typename T>
HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Matrix4x4<T> Transpose(const Matrix4x4<T>& lhs);

template<FloatingPoint T>
HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Matrix4x4<T> GetPerspectiveMatrix(T FOV,
                                                                          T AspectRation,
                                                                          T NearClipZ,
                                                                          T FarClipZ) {
    auto HalfFov = static_cast<T>(FOV / 2.0);
    auto PerTan = static_cast<T>(1.0 / std::tan(HalfFov));
    auto ClipRange = FarClipZ / (FarClipZ - NearClipZ);
    return Matrix4x4<T>({PerTan / AspectRation, 0, 0, 0}, {0, PerTan, 0, 0}, {0, 0, ClipRange, 1},
                        {0, 0, ClipRange * -NearClipZ, 0});
}

}// namespace HUH