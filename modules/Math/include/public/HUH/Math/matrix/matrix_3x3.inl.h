#pragma once

#include <HUH/Math/fwd.h>
#include <HUH/Math/matrix_functions.h>

namespace HUH {
template<typename T>
class Matrix<T, 3, 3> {
public:
    using ValueType = T;
    using RowType = Vector<T, 3>;
    using ColumnType = Vector<T, 3>;
    RowType data[3];

    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE static Matrix Identity() noexcept {
        return {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
    }

    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Matrix() noexcept : data{0} {}
    HUH_CONSTEXPR_FORCE Matrix(const Matrix& other) noexcept = default;
    HUH_CONSTEXPR_FORCE Matrix(Matrix&& other) noexcept = default;
    HUH_CONSTEXPR_FORCE Matrix& operator=(const Matrix& other) noexcept = default;
    HUH_CONSTEXPR_FORCE Matrix& operator=(Matrix&& other) noexcept = default;

    template<typename... U,
             std::enable_if_t<1 + sizeof...(U) == 9, bool> = true,
             std::enable_if_t<(std::is_same_v<T, U> && ...), bool> = true>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Matrix(T t, U... u) noexcept {
        const T data_tmp[9](t, u...);
        std::memcpy(data, data_tmp, sizeof(T) * 9);
    }

    template<std::size_t size, std::enable_if_t<size == 9, bool> = true>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Matrix(const T (&t)[size]) noexcept
        : data{{t[0], t[1], t[2]}, {t[3], t[4], t[5]}, {t[6], t[7], t[8]}} {}

    template<std::size_t size, std::enable_if_t<size == 3, bool> = true>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Matrix(const RowType (&Rows)[size]) noexcept
        : data{Rows[0], Rows[1], Rows[2]} {}

    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Matrix(const RowType& r1, const RowType& r2, const RowType& r3) noexcept
        : data{r1, r2, r3} {}

    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Matrix(T v) noexcept : data{v, v, v, v} {}

    HUH_NODISCARD HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE RowType& operator[](std::size_t index) noexcept {
        return data[index];
    }
    HUH_NODISCARD HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE const RowType& operator[](std::size_t index) const noexcept {
        return data[index];
    }

    HUH_NODISCARD HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE static size_t RowSize() noexcept { return 3; }
    HUH_NODISCARD HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE static size_t ColumnSize() noexcept { return 3; }
};
}// namespace HUH