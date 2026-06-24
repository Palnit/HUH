#pragma once

#include <HUH/Math/fwd.h>
#include <HUH/Math/matrix_functions.h>
#include <cstring>

#if HUH_USE_SIMD
#include <HUH/Simd/register.h>
#include <HUH/Simd/shuffle.h>
#endif

namespace HUH {

///
/// @ingroup Math
/// @class Matrix
/// @tparam T
template<typename T>
class Matrix<T, 4, 4> {
public:
    using ValueType = T;
    using RowType = Vector<T, 4>;
    using ColumnType = Vector<T, 4>;
    RowType data[4];
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE static Matrix Identity() noexcept {
        return {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
    }

    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Matrix() noexcept : data{0} {}
    HUH_CONSTEXPR_FORCE Matrix(const Matrix& other) noexcept = default;
    HUH_CONSTEXPR_FORCE Matrix(Matrix&& other) noexcept = default;
    HUH_CONSTEXPR_FORCE Matrix& operator=(const Matrix& other) noexcept = default;
    HUH_CONSTEXPR_FORCE Matrix& operator=(Matrix&& other) noexcept = default;

    template<typename... U,
             std::enable_if_t<1 + sizeof...(U) == 16, bool> = true,
             std::enable_if_t<(std::is_same_v<T, U> && ...), bool> = true>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Matrix(T t, U... u) noexcept {
        const T data_tmp[16](t, u...);
        std::memcpy(data, data_tmp, sizeof(T) * 16);
    }

    template<std::size_t size, std::enable_if_t<size == 16, bool> = true>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Matrix(const T (&t)[size]) noexcept
        : data{{t[0], t[1], t[2], t[3]},
               {t[4], t[5], t[6], t[7]},
               {t[8], t[9], t[10], t[11]},
               {t[12], t[13], t[14], t[15]}} {}

    template<std::size_t size, std::enable_if_t<size == 4, bool> = true>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Matrix(const RowType (&Rows)[size]) noexcept
        : data{Rows[0], Rows[1], Rows[2], Rows[3]} {}

    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE
    Matrix(const RowType& r1, const RowType& r2, const RowType& r3, const RowType& r4) noexcept
        : data{r1, r2, r3, r4} {}

    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Matrix(T v) noexcept : data{v, v, v, v} {}

    HUH_NODISCARD HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE RowType& operator[](std::size_t index) noexcept {
        return data[index];
    }
    HUH_NODISCARD HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE const RowType& operator[](std::size_t index) const noexcept {
        return data[index];
    }

    HUH_NODISCARD HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE static size_t RowSize() noexcept { return 4; }
    HUH_NODISCARD HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE static size_t ColumnSize() noexcept { return 4; }

    template<typename T2>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Matrix& operator+=(const Matrix<T2, 4, 4>& rhs) noexcept {
        data[0] += rhs.data[0];
        data[1] += rhs.data[1];
        data[2] += rhs.data[2];
        data[3] += rhs.data[3];
        return *this;
    }

    template<typename T2>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE auto operator+(const Matrix<T2, 4, 4>& rhs) noexcept {
        Matrix<std::common_type_t<T, T2>, 4, 4> result(
            {{data[0] + rhs.data[0]}, {data[1] + rhs.data[1]}, {data[2] + rhs.data[2]}, {data[3] + rhs.data[3]}});
        return result;
    }

    template<typename T2>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Matrix& operator+=(const T2& rhs) noexcept {
        data[0] += rhs;
        data[1] += rhs;
        data[2] += rhs;
        data[3] += rhs;
        return *this;
    }

    template<typename T2>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE auto operator+(const T2& rhs) noexcept {
        Matrix<std::common_type_t<T, T2>, 4, 4> result(
            {{data[0] + rhs}, {data[1] + rhs}, {data[2] + rhs}, {data[3] + rhs}});
        return result;
    }

    template<typename T2>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Matrix& operator-=(const Matrix<T2, 4, 4>& rhs) noexcept {
        data[0] -= rhs.data[0];
        data[1] -= rhs.data[1];
        data[2] -= rhs.data[2];
        data[3] -= rhs.data[3];
        return *this;
    }

    template<typename T2>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE auto operator-(const Matrix<T2, 4, 4>& rhs) noexcept {
        Matrix<std::common_type_t<T, T2>, 4, 4> result(
            {{data[0] - rhs.data[0]}, {data[1] - rhs.data[1]}, {data[2] - rhs.data[2]}, {data[3] - rhs.data[3]}});
        return result;
    }

    template<typename T2>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Matrix& operator-=(const T2& rhs) noexcept {
        data[0] -= rhs;
        data[1] -= rhs;
        data[2] -= rhs;
        data[3] -= rhs;
        return *this;
    }

    template<typename T2>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE auto operator-(const T2& rhs) noexcept {
        Matrix<std::common_type_t<T, T2>, 4, 4> result(
            {{data[0] - rhs}, {data[1] - rhs}, {data[2] - rhs}, {data[3] - rhs}});
        return result;
    }

    template<typename T2>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE auto operator*(const Matrix<T2, 4, 4>& rhs) {
        Matrix<std::common_type_t<T, T2>, 4, 4> result;
        HUH::MatrixMultiply(*this, rhs, result);
        return result;
    }

    template<typename T2>
    Matrix& operator*=(const Matrix<T2, 4, 4>& rhs) noexcept {
        Matrix tmp;
        HUH::MatrixMultiply(*this, rhs, tmp);
        *this = tmp;
        return *this;
    }

    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Matrix& Transpose() {
        *this = HUH::Transpose(*this);
        return *this;
    }

    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Matrix GetTransposed() const { return HUH::Transpose(*this); }

    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Matrix& Inverse() {
        Matrix result;
        HUH::MatrixInverse(*this, result);
        *this = result;
        return *this;
    }

    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Matrix GetInversed() const {
        Matrix result;
        HUH::MatrixInverse(*this, result);
        return result;
    }

    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE static Matrix GetPerspective(T FOV, T AspectRation, T NearClipZ, T FarClipZ)
        requires(FloatingPoint<T>)
    {
        return HUH::GetPerspectiveMatrix(FOV, AspectRation, NearClipZ, FarClipZ);
    }
};

template<typename T, typename T2>
HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE void MatrixMultiply(const Matrix4x4<T>& lhs,
                                                            const Matrix4x4<T2>& rhs,
                                                            Matrix4x4<std::common_type_t<T, T2>>& result) {

    typename Matrix4x4<T>::RowType lhsRow0 = lhs[0];
    typename Matrix4x4<T>::RowType lhsRow1 = lhs[1];
    typename Matrix4x4<T>::RowType lhsRow2 = lhs[2];
    typename Matrix4x4<T>::RowType lhsRow3 = lhs[3];

    typename Matrix4x4<T2>::RowType rhsRow0 = rhs[0];
    typename Matrix4x4<T2>::RowType rhsRow1 = rhs[1];
    typename Matrix4x4<T2>::RowType rhsRow2 = rhs[2];
    typename Matrix4x4<T2>::RowType rhsRow3 = rhs[3];

    result[0] = rhsRow0 * lhsRow0[0] + rhsRow1 * lhsRow0[1] + rhsRow2 * lhsRow0[2] + rhsRow3 * lhsRow0[3];
    result[1] = rhsRow0 * lhsRow1[0] + rhsRow1 * lhsRow1[1] + rhsRow2 * lhsRow1[2] + rhsRow3 * lhsRow1[3];
    result[2] = rhsRow0 * lhsRow2[0] + rhsRow1 * lhsRow2[1] + rhsRow2 * lhsRow2[2] + rhsRow3 * lhsRow2[3];
    result[3] = rhsRow0 * lhsRow3[0] + rhsRow1 * lhsRow3[1] + rhsRow2 * lhsRow3[2] + rhsRow3 * lhsRow3[3];
}

template<typename T>
HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE void MatrixInverse(const Matrix4x4<T>& mat, Matrix4x4<T>& result) {
    T SubFactor00 = mat[2][2] * mat[3][3] - mat[3][2] * mat[2][3];
    T SubFactor01 = mat[2][1] * mat[3][3] - mat[3][1] * mat[2][3];
    T SubFactor02 = mat[2][1] * mat[3][2] - mat[3][1] * mat[2][2];
    T SubFactor03 = mat[2][0] * mat[3][3] - mat[3][0] * mat[2][3];
    T SubFactor04 = mat[2][0] * mat[3][2] - mat[3][0] * mat[2][2];
    T SubFactor05 = mat[2][0] * mat[3][1] - mat[3][0] * mat[2][1];
    T SubFactor06 = mat[1][2] * mat[3][3] - mat[3][2] * mat[1][3];
    T SubFactor07 = mat[1][1] * mat[3][3] - mat[3][1] * mat[1][3];
    T SubFactor08 = mat[1][1] * mat[3][2] - mat[3][1] * mat[1][2];
    T SubFactor09 = mat[1][0] * mat[3][3] - mat[3][0] * mat[1][3];
    T SubFactor10 = mat[1][0] * mat[3][2] - mat[3][0] * mat[1][2];
    T SubFactor11 = mat[1][0] * mat[3][1] - mat[3][0] * mat[1][1];
    T SubFactor12 = mat[1][2] * mat[2][3] - mat[2][2] * mat[1][3];
    T SubFactor13 = mat[1][1] * mat[2][3] - mat[2][1] * mat[1][3];
    T SubFactor14 = mat[1][1] * mat[2][2] - mat[2][1] * mat[1][2];
    T SubFactor15 = mat[1][0] * mat[2][3] - mat[2][0] * mat[1][3];
    T SubFactor16 = mat[1][0] * mat[2][2] - mat[2][0] * mat[1][2];
    T SubFactor17 = mat[1][0] * mat[2][1] - mat[2][0] * mat[1][1];

    result[0][0] = +(mat[1][1] * SubFactor00 - mat[1][2] * SubFactor01 + mat[1][3] * SubFactor02);
    result[0][1] = -(mat[1][0] * SubFactor00 - mat[1][2] * SubFactor03 + mat[1][3] * SubFactor04);
    result[0][2] = +(mat[1][0] * SubFactor01 - mat[1][1] * SubFactor03 + mat[1][3] * SubFactor05);
    result[0][3] = -(mat[1][0] * SubFactor02 - mat[1][1] * SubFactor04 + mat[1][2] * SubFactor05);

    result[1][0] = -(mat[0][1] * SubFactor00 - mat[0][2] * SubFactor01 + mat[0][3] * SubFactor02);
    result[1][1] = +(mat[0][0] * SubFactor00 - mat[0][2] * SubFactor03 + mat[0][3] * SubFactor04);
    result[1][2] = -(mat[0][0] * SubFactor01 - mat[0][1] * SubFactor03 + mat[0][3] * SubFactor05);
    result[1][3] = +(mat[0][0] * SubFactor02 - mat[0][1] * SubFactor04 + mat[0][2] * SubFactor05);

    result[2][0] = +(mat[0][1] * SubFactor06 - mat[0][2] * SubFactor07 + mat[0][3] * SubFactor08);
    result[2][1] = -(mat[0][0] * SubFactor06 - mat[0][2] * SubFactor09 + mat[0][3] * SubFactor10);
    result[2][2] = +(mat[0][0] * SubFactor07 - mat[0][1] * SubFactor09 + mat[0][3] * SubFactor11);
    result[2][3] = -(mat[0][0] * SubFactor08 - mat[0][1] * SubFactor10 + mat[0][2] * SubFactor11);

    result[3][0] = -(mat[0][1] * SubFactor12 - mat[0][2] * SubFactor13 + mat[0][3] * SubFactor14);
    result[3][1] = +(mat[0][0] * SubFactor12 - mat[0][2] * SubFactor15 + mat[0][3] * SubFactor16);
    result[3][2] = -(mat[0][0] * SubFactor13 - mat[0][1] * SubFactor15 + mat[0][3] * SubFactor17);
    result[3][3] = +(mat[0][0] * SubFactor14 - mat[0][1] * SubFactor16 + mat[0][2] * SubFactor17);

    T Determinant =
        +mat[0][0] * result[0][0] + mat[0][1] * result[0][1] + mat[0][2] * result[0][2] + mat[0][3] * result[0][3];

    for (size_t i = 0; i < 4; i++) {
        result[i] /= Determinant;
    }
}

#if HUH_USE_SIMD
HUH_FORCE_INLINE void MatrixMultiply(const Matrix4x4<float>& lhs,
                                     const Matrix4x4<float>& rhs,
                                     Matrix4x4<float>& result) {

    const Simd::Register<float, 4> rhsRow0(rhs[0].data);
    const Simd::Register<float, 4> rhsRow1(rhs[1].data);
    const Simd::Register<float, 4> rhsRow2(rhs[2].data);
    const Simd::Register<float, 4> rhsRow3(rhs[3].data);

    const Simd::Register<float, 4> lhsRow0(lhs[0].data);
    const Simd::Register<float, 4> lhsRow1(lhs[1].data);
    const Simd::Register<float, 4> lhsRow2(lhs[2].data);
    const Simd::Register<float, 4> lhsRow3(lhs[3].data);

    Simd::Register<float, 4> tmp(HUH::Simd::ShuffleRegister<0>(lhsRow0) * rhsRow0);
    tmp = HUH::Simd::FMA(HUH::Simd::ShuffleRegister<1>(lhsRow0), rhsRow1, tmp);
    tmp = HUH::Simd::FMA(HUH::Simd::ShuffleRegister<2>(lhsRow0), rhsRow2, tmp);
    HUH::Simd::FMA(HUH::Simd::ShuffleRegister<3>(lhsRow0), rhsRow3, tmp).Store(result[0].data);

    tmp = HUH::Simd::ShuffleRegister<0>(lhsRow1) * rhsRow0;
    tmp = HUH::Simd::FMA(HUH::Simd::ShuffleRegister<1>(lhsRow1), rhsRow1, tmp);
    tmp = HUH::Simd::FMA(HUH::Simd::ShuffleRegister<2>(lhsRow1), rhsRow2, tmp);
    HUH::Simd::FMA(HUH::Simd::ShuffleRegister<3>(lhsRow1), rhsRow3, tmp).Store(result[1].data);

    tmp = HUH::Simd::ShuffleRegister<0>(lhsRow2) * rhsRow0;
    tmp = HUH::Simd::FMA(HUH::Simd::ShuffleRegister<1>(lhsRow2), rhsRow1, tmp);
    tmp = HUH::Simd::FMA(HUH::Simd::ShuffleRegister<2>(lhsRow2), rhsRow2, tmp);
    HUH::Simd::FMA(HUH::Simd::ShuffleRegister<3>(lhsRow2), rhsRow3, tmp).Store(result[2].data);

    tmp = HUH::Simd::ShuffleRegister<0>(lhsRow3) * rhsRow0;
    tmp = HUH::Simd::FMA(HUH::Simd::ShuffleRegister<1>(lhsRow3), rhsRow1, tmp);
    tmp = HUH::Simd::FMA(HUH::Simd::ShuffleRegister<2>(lhsRow3), rhsRow2, tmp);
    HUH::Simd::FMA(HUH::Simd::ShuffleRegister<3>(lhsRow3), rhsRow3, tmp).Store(result[3].data);
}
#endif

template<typename T>
HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Matrix4x4<T> Transpose(const Matrix4x4<T>& lhs) {
    return Matrix4x4<T>({{lhs[0][0], lhs[1][0], lhs[2][0], lhs[3][0]},
                         {lhs[0][1], lhs[1][1], lhs[2][1], lhs[3][1]},
                         {lhs[0][2], lhs[1][2], lhs[2][2], lhs[3][2]},
                         {lhs[0][3], lhs[1][3], lhs[2][3], lhs[3][3]}});
}

}// namespace HUH
