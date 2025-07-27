#pragma once

#include <HUH/Math/fwd.h>
#include <HUH/definitions.h>
#include <cmath>

namespace HUH {

template<typename T, typename T2, std::size_t R, std::size_t C, std::size_t C2>
HUH_CONSTEXPR_FORCE void MatrixMultiply(Matrix<std::common_type_t<T, T2>, R, C2>& result,
                                        const Matrix<T, R, C>& lhs,
                                        const Matrix<T, C, C2>& rhs) {
    for (std::size_t i = 0; i < R; i++) {
        for (std::size_t k = 0; k < C; k++) {
            for (std::size_t j = 0; j < C2; j++) {
                result.data[i][j] = std::fma(lhs.data[i][k], rhs.data[k][j], result.data[i][j]);
            }
        }
    }
}

}// namespace HUH