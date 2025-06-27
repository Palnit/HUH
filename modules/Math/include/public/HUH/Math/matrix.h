#pragma once

#include <cstddef>

#include <HUH/Math/vector.h>

#define MATRIX_PARALLEL_FOR_OVER_ROWS(Body)                   \
    oneapi::tbb::parallel_for(static_cast<std::size_t>(0), N, \
                              [&](std::size_t i) { Body });

namespace HUH {

template<typename T,
         std::size_t N,
         std::size_t M,
         std::enable_if_t<std::is_arithmetic_v<T>, bool> = true,
         std::enable_if_t<(N > 0) && (M > 0), bool> = true>
class Matrix {

public:
    using RowType = Vector<T, M>;
    using ColumnType = Vector<T, N>;
    Vector<T, M> data[N];

    Matrix() noexcept : data{0} {}
    Matrix(const Matrix& other) noexcept = default;
    Matrix(Matrix&& other) noexcept = default;
    Matrix& operator=(const Matrix& other) noexcept = default;
    Matrix& operator=(Matrix&& other) noexcept = default;

    template<typename... U,
             std::enable_if_t<1 + sizeof...(U) == N * M, bool> = true,
             std::enable_if_t<(std::is_same_v<T, U> && ...), bool> = true>
    Matrix(T t, U... u) noexcept {
        T data_tmp[N * M](t, u...);
        std::memcpy(data, data_tmp, sizeof(T) * N * M);
    }

    template<std::size_t size, std::enable_if_t<size == N * M, bool> = true>
    Matrix(const T (&t)[size]) noexcept : data(*t) {
        std::memcpy(data, t, sizeof(T) * N * M);
    }

    template<std::size_t size, std::enable_if_t<size == N, bool> = true>
    Matrix(const Vector<T, M> (&Rows)[size]) noexcept {
        std::memcpy(data, Rows, sizeof(T) * N * M);
    }

    Matrix(T v) noexcept : data{0} {
        MATRIX_PARALLEL_FOR_OVER_ROWS(data[i] = v;)
    }

    Vector<T, M>& operator[](std::size_t index) noexcept { return data[index]; }
    const Vector<T, M>& operator[](std::size_t index) const noexcept {
        return data[index];
    }

    Matrix& operator+=(const Matrix& rhs) noexcept {
        MATRIX_PARALLEL_FOR_OVER_ROWS(data[i] += rhs.data[i];)
        return *this;
    }

    friend Matrix operator+(Matrix lhs, const Matrix& rhs) noexcept {
        MATRIX_PARALLEL_FOR_OVER_ROWS(lhs.data[i] += rhs.data[i];)
        return lhs;
    }

    Matrix& operator-=(const Matrix& rhs) noexcept {
        MATRIX_PARALLEL_FOR_OVER_ROWS(data[i] -= rhs.data[i];)
        return *this;
    }

    friend Matrix operator-(Matrix lhs, const Matrix& rhs) noexcept {
        MATRIX_PARALLEL_FOR_OVER_ROWS(lhs.data[i] -= rhs.data[i];)
        return lhs;
    }

    // template<typename T2,
    //          std::size_t N2,
    //          std::size_t M2,
    //          std::enable_if_t<std::is_same_v<T, T2>, bool> = true,
    //          std::enable_if_t<N == M2, bool> = true>
    // Matrix<T, N2, M> operator*(Matrix lhs, const Matrix<T2, N2, M2>& rhs) {
    //     // oneapi::tbb::parallel_for(
    //     //     oneapi::tbb::blocked_range3d<std::size_t>(0, N2, 0)) return lhs;
    // }
};

template<typename T, std::size_t N, std::size_t M>
std::ostream& operator<<(std::ostream& os, const Matrix<T, N, M>& mat) {
    os << "[ " << mat[0];
    for (std::size_t i = 1; i < N; i++) { os << ", " << mat[i]; }
    os << "]";
    return os;
}

template<std::size_t I, typename T, std::size_t N, std::size_t M>
constexpr const std::tuple_element_t<I, Matrix<T, N, M>>& get(
    const Matrix<T, N, M>& Mat) noexcept {
    return Mat[I];
}

template<std::size_t I, typename T, std::size_t N, std::size_t M>
constexpr std::tuple_element_t<I, Matrix<T, N, M>>& get(
    Matrix<T, N, M>& Mat) noexcept {
    return Mat[I];
}

template<std::size_t I, typename T, std::size_t N, std::size_t M>
constexpr const std::tuple_element_t<I, Matrix<T, N, M>>&& get(
    const Matrix<T, N, M>&& Mat) noexcept {
    return std::move(Mat[I]);
}

template<std::size_t I, typename T, std::size_t N, std::size_t M>
constexpr std::tuple_element_t<I, Matrix<T, N, M>>&& get(
    Matrix<T, N, M>&& Mat) noexcept {
    return std::move(Mat[I]);
}

template<typename T, std::size_t N, std::size_t M>
Vector<T, M>* begin(Matrix<T, N, M>& Mat) {
    return Mat.data;
}

template<typename T, std::size_t N, std::size_t M>
Vector<T, M>* end(Matrix<T, N, M>& Mat) {
    return Mat.data + N;
}

}// namespace HUH

template<typename T, std::size_t N, std::size_t M>
struct std::tuple_size<HUH::Matrix<T, N, M>>
    : public integral_constant<std::size_t, N> {};

template<std::size_t I, typename T, std::size_t N, std::size_t M>
struct std::tuple_element<I, HUH::Matrix<T, N, M>> {
    using type = HUH::Vector<T, M>;
};
