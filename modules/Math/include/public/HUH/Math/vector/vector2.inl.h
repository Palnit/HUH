#pragma once

#include <type_traits>

template<typename T>
class HUH::Vector<T, 2> {

public:
    T data[2];

    HUH_CONSTEXPR_FORCE Vector() noexcept : data{0} {}
    HUH_CONSTEXPR_FORCE Vector(const Vector& other) noexcept = default;
    HUH_CONSTEXPR_FORCE Vector(Vector&& other) noexcept = default;
    HUH_CONSTEXPR_FORCE Vector& operator=(const Vector& other) noexcept =
        default;
    HUH_CONSTEXPR_FORCE Vector& operator=(Vector&& other) noexcept = default;

    HUH_CONSTEXPR_FORCE Vector(const T& x, const T& y) noexcept : data{x, y} {}

    HUH_CONSTEXPR_FORCE Vector(const T& v) noexcept : data{v, v} {}

    template<std::size_t size, std::enable_if_t<size == 2, bool> = true>
    HUH_CONSTEXPR_FORCE Vector(const T (&t)[size]) noexcept
        : data{t[0], t[1]} {}

    HUH_NODISCARD HUH_CONSTEXPR_FORCE T& X() noexcept { return data[0]; }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE T& Y() noexcept { return data[1]; }

    HUH_NODISCARD HUH_CONSTEXPR_FORCE const T& X() const noexcept {
        return data[0];
    }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE const T& Y() const noexcept {
        return data[1];
    }

    HUH_NODISCARD HUH_CONSTEXPR_FORCE T& U() noexcept { return data[0]; }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE T& V() noexcept { return data[1]; }

    HUH_NODISCARD HUH_CONSTEXPR_FORCE const T& U() const noexcept {
        return data[0];
    }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE const T& V() const noexcept {
        return data[1];
    }

    HUH_NODISCARD HUH_CONSTEXPR_FORCE T* XY() noexcept { return data; }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE const T* XY() const noexcept {
        return data;
    }

    HUH_NODISCARD HUH_CONSTEXPR_FORCE T* UV() noexcept { return data; }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE const T* UV() const noexcept {
        return data;
    }

    HUH_NODISCARD HUH_CONSTEXPR_FORCE T& operator[](
        std::size_t index) noexcept {
        return data[index];
    }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE const T& operator[](
        std::size_t index) const noexcept {
        return data[index];
    }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE static std::size_t Size() { return 2; }

    HUH_CONSTEXPR_FORCE Vector& operator+=(const Vector& rhs) noexcept {
        data[0] += rhs.data[0];
        data[1] += rhs.data[1];
        return *this;
    }

    HUH_CONSTEXPR_FORCE friend Vector operator+(Vector lhs,
                                                const Vector& rhs) noexcept {
        lhs.data[0] += rhs.data[0];
        lhs.data[1] += rhs.data[1];
        return lhs;
    }

    HUH_CONSTEXPR_FORCE Vector& operator-=(const Vector& rhs) noexcept {
        data[0] -= rhs.data[0];
        data[1] -= rhs.data[1];
        return *this;
    }

    HUH_CONSTEXPR_FORCE friend Vector operator-(Vector lhs,
                                                const Vector& rhs) noexcept {
        lhs.data[0] -= rhs.data[0];
        lhs.data[1] -= rhs.data[1];
        return lhs;
    }

    HUH_CONSTEXPR_FORCE Vector operator-() noexcept {
        return {-data[0], -data[1]};
    }

    HUH_CONSTEXPR_FORCE Vector& operator*=(const Vector& rhs) noexcept {
        data[0] *= rhs.data[0];
        data[1] *= rhs.data[1];
        return *this;
    }

    HUH_CONSTEXPR_FORCE friend Vector operator*(Vector lhs,
                                                const Vector& rhs) noexcept {
        lhs.data[0] *= rhs.data[0];
        lhs.data[1] *= rhs.data[1];
        return lhs;
    }

    HUH_CONSTEXPR_FORCE Vector& operator*=(const T& rhs) noexcept {
        data[0] *= rhs;
        data[1] *= rhs;
        return *this;
    }

    HUH_CONSTEXPR_FORCE friend Vector operator*(Vector lhs,
                                                const T& rhs) noexcept {
        lhs.data[0] *= rhs;
        lhs.data[1] *= rhs;
        return lhs;
    }

    HUH_CONSTEXPR_FORCE Vector& operator/=(const Vector& rhs) noexcept {
        data[0] /= rhs.data[0];
        data[1] /= rhs.data[1];
        return *this;
    }

    HUH_CONSTEXPR_FORCE friend Vector operator/(Vector lhs,
                                                const Vector& rhs) noexcept {
        lhs.data[0] /= rhs.data[0];
        lhs.data[1] /= rhs.data[1];
        return lhs;
    }

    HUH_CONSTEXPR_FORCE Vector& operator/=(const T& rhs) noexcept {
        data[0] /= rhs;
        data[1] /= rhs;
        return *this;
    }

    HUH_CONSTEXPR_FORCE friend Vector operator/(Vector lhs,
                                                const T& rhs) noexcept {
        lhs.data[0] /= rhs;
        lhs.data[1] /= rhs;
        return lhs;
    }
};
