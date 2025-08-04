#pragma once

#include <HUH/Math/fwd.h>
#include <HUH/definitions.h>
#include <cmath>

namespace HUH {

template<typename T, typename T2, std::size_t R, std::size_t C, std::size_t C2>
HUH_CONSTEXPR_FORCE void MatrixMultiply(const Matrix<T, R, C>& lhs,
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
HUH_CONSTEXPR_FORCE void MatrixMultiply(const Matrix4x4<T>& lhs,
                                        const Matrix4x4<T2>& rhs,
                                        Matrix4x4<std::common_type_t<T, T2>>& result);

#ifdef HUH_USE_SIMD
HUH_FORCE_INLINE void MatrixMultiply(const Matrix4x4<float>& lhs,
                                     const Matrix4x4<float>& rhs,
                                     Matrix4x4<float>& result);
#endif

template<typename T, size_t R, size_t C>
HUH_CONSTEXPR_FORCE Matrix<T, C, R> Transpose(const Matrix<T, R, C>& lhs) {
    std::cout << "Transpose() R C" << std::endl;
    return Matrix<T, C, R>();
}

template<typename T, size_t N>
HUH_CONSTEXPR_FORCE Matrix<T, N, N> Transpose(const Matrix<T, N, N>& lhs) {
    std::cout << "Transpose() N" << std::endl;
    return Matrix<T, N, N>(lhs.data);
}

template<typename T>
HUH_CONSTEXPR_FORCE Matrix4x4<T> Transpose(const Matrix4x4<T>& lhs);

}// namespace HUH