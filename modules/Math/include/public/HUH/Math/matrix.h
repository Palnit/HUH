#pragma once

#include <HUH/Math/vector.h>
#include <cstddef>

#include "matrix_functions.h"

namespace HUH {

template<typename T, std::size_t R, std::size_t C>
class Matrix {
    static_assert(std::is_arithmetic_v<T>, "T must be arithmetic type");
    static_assert(R > 1 && C > 1,
                  "Matrix must be bigger than 2x2 if you want a 1 dimensional matrix use vectors dummy");

public:
    using ValueType = T;
    using RowType = Vector<T, C>;
    using ColumnType = Vector<T, R>;
    RowType data[R];

    HUH_CONSTEXPR_FORCE Matrix() noexcept : data{0} {}
    HUH_CONSTEXPR_FORCE Matrix(const Matrix& other) noexcept = default;
    HUH_CONSTEXPR_FORCE Matrix(Matrix&& other) noexcept = default;
    HUH_CONSTEXPR_FORCE Matrix& operator=(const Matrix& other) noexcept = default;
    HUH_CONSTEXPR_FORCE Matrix& operator=(Matrix&& other) noexcept = default;

    template<typename... U,
             std::enable_if_t<1 + sizeof...(U) == R * C, bool> = true,
             std::enable_if_t<(std::is_same_v<T, U> && ...), bool> = true>
    HUH_CONSTEXPR_FORCE Matrix(T t, U... u) noexcept {
        const T data_tmp[R * C](t, u...);
        std::memcpy(data, data_tmp, sizeof(T) * R * C);
    }

    template<std::size_t size, std::enable_if_t<size == R * C, bool> = true>
    HUH_CONSTEXPR_FORCE Matrix(const T (&t)[size]) noexcept {
        std::memcpy(data, t, sizeof(T) * R * C);
    }

    template<std::size_t size, std::enable_if_t<size == R, bool> = true>
    HUH_CONSTEXPR_FORCE Matrix(const RowType (&Rows)[size]) noexcept {
        std::memcpy(data, Rows, sizeof(T) * R * C);
    }

    HUH_CONSTEXPR_FORCE Matrix(T v) noexcept : data{0} {
        for (std::size_t i = 0; i < R; i++) {
            data[i] = v;
        }
    }

    HUH_NODISCARD HUH_CONSTEXPR_FORCE RowType& operator[](std::size_t index) noexcept { return data[index]; }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE const RowType& operator[](std::size_t index) const noexcept {
        return data[index];
    }

    HUH_NODISCARD HUH_CONSTEXPR_FORCE static size_t RowSize() noexcept { return R; }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE static size_t ColumnSize() noexcept { return C; }

    template<typename T2>
    HUH_CONSTEXPR_FORCE Matrix& operator+=(const Matrix<T2, R, C>& rhs) noexcept {
        for (std::size_t i = 0; i < R; i++) {
            data[i] += rhs.data[i];
        }
        return *this;
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE auto operator+(const Matrix<T2, R, C>& rhs) noexcept {
        Matrix<std::common_type_t<T, T2>, R, C> result;
        for (std::size_t i = 0; i < R; i++) {
            result.data[i] = data[i] + rhs.data[i];
        }
        return result;
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE Matrix& operator+=(const T2& rhs) noexcept {
        for (std::size_t i = 0; i < R; i++) {
            data[i] += rhs;
        }
        return *this;
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE auto operator+(const T2& rhs) noexcept {
        Matrix<std::common_type_t<T, T2>, R, C> result;
        for (std::size_t i = 0; i < R; i++) {
            result.data[i] = data[i] + rhs;
        }
        return result;
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE Matrix& operator-=(const Matrix<T2, R, C>& rhs) noexcept {
        for (std::size_t i = 0; i < R; i++) {
            data[i] -= rhs.data[i];
        }
        return *this;
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE auto operator-(const Matrix<T2, R, C>& rhs) noexcept {
        Matrix<std::common_type_t<T, T2>, R, C> result;
        for (std::size_t i = 0; i < R; i++) {
            result.data[i] = data[i] - rhs.data[i];
        }
        return result;
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE Matrix& operator-=(const T2& rhs) noexcept {
        for (std::size_t i = 0; i < R; i++) {
            data[i] -= rhs;
        }
        return *this;
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE auto operator-(const T2& rhs) noexcept {
        Matrix<std::common_type_t<T, T2>, R, C> result;
        for (std::size_t i = 0; i < R; i++) {
            result.data[i] = data[i] - rhs;
        }
        return result;
    }

    template<typename T2, std::size_t C2>
    HUH_CONSTEXPR_FORCE auto operator*(const Matrix<T2, C, C2>& rhs) {
        Matrix<std::common_type_t<T, T2>, R, C2> result{};
        HUH::MatrixMultiply(*this, rhs, result);
        return result;
    }

    template<typename T2>
    Matrix& operator*=(const Matrix<T2, R, C>& rhs) noexcept {
        Matrix tmp;
        HUH::MatrixMultiply(*this, rhs, tmp);
        *this = tmp;
        return *this;
    }
};

template<typename T, std::size_t C, std::size_t R>
std::ostream& operator<<(std::ostream& os, const Matrix<T, C, R>& mat) {
    os << "[ " << mat[0];
    for (std::size_t i = 1; i < C; i++) {
        os << ", " << mat[i];
    }
    os << "]";
    return os;
}

template<std::size_t I, typename T, std::size_t C, std::size_t R>
constexpr const std::tuple_element_t<I, Matrix<T, C, R>>& get(const Matrix<T, C, R>& Mat) noexcept {
    return Mat[I];
}

template<std::size_t I, typename T, std::size_t C, std::size_t R>
constexpr std::tuple_element_t<I, Matrix<T, C, R>>& get(Matrix<T, C, R>& Mat) noexcept {
    return Mat[I];
}

template<std::size_t I, typename T, std::size_t C, std::size_t R>
constexpr const std::tuple_element_t<I, Matrix<T, C, R>>&& get(const Matrix<T, C, R>&& Mat) noexcept {
    return std::move(Mat[I]);
}

template<std::size_t I, typename T, std::size_t C, std::size_t R>
constexpr std::tuple_element_t<I, Matrix<T, C, R>>&& get(Matrix<T, C, R>&& Mat) noexcept {
    return std::move(Mat[I]);
}

template<typename T, std::size_t C, std::size_t R>
typename Matrix<T, C, R>::RowType* begin(Matrix<T, C, R>& Mat) {
    return Mat.data;
}

template<typename T, std::size_t C, std::size_t R>
typename Matrix<T, C, R>::RowType* end(Matrix<T, C, R>& Mat) {
    return Mat.data + C;
}

}// namespace HUH

template<typename T, std::size_t C, std::size_t R>
struct std::tuple_size<HUH::Matrix<T, C, R>> : public integral_constant<std::size_t, C> {};

template<std::size_t I, typename T, std::size_t C, std::size_t R>
struct std::tuple_element<I, HUH::Matrix<T, C, R>> {
    using type = typename HUH::Matrix<T, C, R>::RowType;
};

#include <HUH/Math/matrix/matrix_4x4.inl.h>