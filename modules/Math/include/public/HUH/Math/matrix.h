#pragma once

#include <HUH/Math/vector.h>
#include <cstddef>

namespace HUH {

template<typename T,
         std::size_t R,
         std::size_t C,
         std::enable_if_t<std::is_arithmetic_v<T>, bool> = true,
         std::enable_if_t<(R > 0) && (C > 0), bool> = true>
class Matrix {
public:
    using RowType = Vector<T, C>;
    using ColumnType = Vector<T, R>;
    RowType data[R];

    Matrix() noexcept : data{0} {}
    Matrix(const Matrix& other) noexcept = default;
    Matrix(Matrix&& other) noexcept = default;
    Matrix& operator=(const Matrix& other) noexcept = default;
    Matrix& operator=(Matrix&& other) noexcept = default;

    template<typename... U,
             std::enable_if_t<1 + sizeof...(U) == R * C, bool> = true,
             std::enable_if_t<(std::is_same_v<T, U> && ...), bool> = true>
    Matrix(T t, U... u) noexcept {
        const T data_tmp[R * C](t, u...);
        std::memcpy(data, data_tmp, sizeof(T) * R * C);
    }

    template<std::size_t size, std::enable_if_t<size == R * C, bool> = true>
    Matrix(const T (&t)[size]) noexcept : data(*t) {
        std::memcpy(data, t, sizeof(T) * R * C);
    }

    template<std::size_t size, std::enable_if_t<size == R, bool> = true>
    Matrix(const RowType (&Rows)[size]) noexcept {
        std::memcpy(data, Rows, sizeof(T) * R * C);
    }

    Matrix(T v) noexcept : data{0} {
        for (std::size_t i = 0; i < R; i++) {
            data[i] = v;
        }
    }

    RowType& operator[](std::size_t index) noexcept { return data[index]; }
    const RowType& operator[](std::size_t index) const noexcept {
        return data[index];
    }

    Matrix& operator+=(const Matrix& rhs) noexcept {
        for (std::size_t i = 0; i < R; i++) {
            data[i] += rhs.data[i];
        }
        return *this;
    }

    friend Matrix operator+(Matrix lhs, const Matrix& rhs) noexcept {
        for (std::size_t i = 0; i < R; i++) {
            lhs.data[i] += rhs.data[i];
        }
        return lhs;
    }

    Matrix& operator-=(const Matrix& rhs) noexcept {
        for (std::size_t i = 0; i < R; i++) {
            data[i] -= rhs.data[i];
        }
        return *this;
    }

    friend Matrix operator-(Matrix lhs, const Matrix& rhs) noexcept {
        for (std::size_t i = 0; i < R; i++) {
            lhs.data[i] -= rhs.data[i];
        }
        return lhs;
    }

    template<std::size_t C2>
    friend Matrix<T, R, C2> operator*(const Matrix& lhs,
                                      const Matrix<T, C, C2>& rhs) {
        Matrix<T, R, C2> result{};
        for (std::size_t i = 0; i < R; i++) {
            for (std::size_t k = 0; k < C; k++) {
                for (std::size_t j = 0; j < C2; j++) {
                    result.data[i][j] += lhs.data[i][k] * rhs.data[k][j];
                }
            }
        }
        return result;
    }

    Matrix& operator*=(const Matrix& rhs) noexcept {
        Matrix tmp(0);
        for (std::size_t i = 0; i < R; i++) {
            for (std::size_t k = 0; k < R; k++) {
                for (std::size_t j = 0; j < C; j++) {
                    tmp[i][j] += data[i][k] * rhs.data[k][j];
                }
            }
        }
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
constexpr const std::tuple_element_t<I, Matrix<T, C, R>>& get(
    const Matrix<T, C, R>& Mat) noexcept {
    return Mat[I];
}

template<std::size_t I, typename T, std::size_t C, std::size_t R>
constexpr std::tuple_element_t<I, Matrix<T, C, R>>& get(
    Matrix<T, C, R>& Mat) noexcept {
    return Mat[I];
}

template<std::size_t I, typename T, std::size_t C, std::size_t R>
constexpr const std::tuple_element_t<I, Matrix<T, C, R>>&& get(
    const Matrix<T, C, R>&& Mat) noexcept {
    return std::move(Mat[I]);
}

template<std::size_t I, typename T, std::size_t C, std::size_t R>
constexpr std::tuple_element_t<I, Matrix<T, C, R>>&& get(
    Matrix<T, C, R>&& Mat) noexcept {
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
struct std::tuple_size<HUH::Matrix<T, C, R>>
    : public integral_constant<std::size_t, C> {};

template<std::size_t I, typename T, std::size_t C, std::size_t R>
struct std::tuple_element<I, HUH::Matrix<T, C, R>> {
    using type = typename HUH::Matrix<T, C, R>::RowType;
};
