#pragma once

#include <HUH/Math/fwd.h>
#include <HUH/Math/matrix_functions.h>
#include <cstring>

namespace HUH {

template<typename T>
class Matrix<T, 4, 4> {
public:
    using RowType = Vector<T, 4>;
    using ColumnType = Vector<T, 4>;
    RowType data[4];

    HUH_CONSTEXPR_FORCE Matrix() noexcept : data{0} {}
    HUH_CONSTEXPR_FORCE Matrix(const Matrix& other) noexcept = default;
    HUH_CONSTEXPR_FORCE Matrix(Matrix&& other) noexcept = default;
    HUH_CONSTEXPR_FORCE Matrix& operator=(const Matrix& other) noexcept = default;
    HUH_CONSTEXPR_FORCE Matrix& operator=(Matrix&& other) noexcept = default;

    template<typename... U,
             std::enable_if_t<1 + sizeof...(U) == 16, bool> = true,
             std::enable_if_t<(std::is_same_v<T, U> && ...), bool> = true>
    HUH_CONSTEXPR_FORCE Matrix(T t, U... u) noexcept {
        const T data_tmp[16](t, u...);
        std::memcpy(data, data_tmp, sizeof(T) * 16);
    }

    template<std::size_t size, std::enable_if_t<size == 16, bool> = true>
    HUH_CONSTEXPR_FORCE Matrix(const T (&t)[size]) noexcept
        : data{{t[0], t[1], t[2], t[3]},
               {t[4], t[5], t[6], t[7]},
               {t[8], t[9], t[10], t[11]},
               {t[12], t[13], t[14], t[15]}} {}

    template<std::size_t size, std::enable_if_t<size == 4, bool> = true>
    HUH_CONSTEXPR_FORCE Matrix(const RowType (&Rows)[size]) noexcept : data{Rows[0], Rows[1], Rows[2], Rows[3]} {}

    HUH_CONSTEXPR_FORCE Matrix(T v) noexcept : data{v, v, v, v} {}

    HUH_NODISCARD HUH_CONSTEXPR_FORCE RowType& operator[](std::size_t index) noexcept { return data[index]; }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE const RowType& operator[](std::size_t index) const noexcept {
        return data[index];
    }
};
}// namespace HUH
