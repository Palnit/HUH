#pragma once

#include <type_traits>

template<typename T>
class HUH::Vector<T, 3> {

public:
    T data[3];

    HUH_CONSTEXPR_FORCE Vector() noexcept : data{} {}
    HUH_CONSTEXPR_FORCE Vector(const Vector& other) noexcept = default;
    HUH_CONSTEXPR_FORCE Vector(Vector&& other) noexcept = default;
    HUH_CONSTEXPR_FORCE Vector& operator=(const Vector& other) noexcept =
        default;
    HUH_CONSTEXPR_FORCE Vector& operator=(Vector&& other) noexcept = default;

    HUH_CONSTEXPR_FORCE Vector(const T& x, const T& y, const T& z) noexcept
        : data{x, y, z} {}
    HUH_CONSTEXPR_FORCE Vector(const T& v) noexcept : data{v, v, v} {}

    template<std::size_t size, std::enable_if_t<size == 3, bool> = true>
    HUH_CONSTEXPR_FORCE Vector(const T (&t)[size]) noexcept
        : data{t[0], t[1], t[2]} {}

    HUH_NODISCARD HUH_CONSTEXPR_FORCE T& X() noexcept { return data[0]; }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE T& Y() noexcept { return data[1]; }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE T& Z() noexcept { return data[2]; }

    HUH_NODISCARD HUH_CONSTEXPR_FORCE const T& X() const noexcept {
        return data[0];
    }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE const T& Y() const noexcept {
        return data[1];
    }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE const T& Z() const noexcept {
        return data[2];
    }

    HUH_NODISCARD HUH_CONSTEXPR_FORCE T& R() noexcept { return data[0]; }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE T& G() noexcept { return data[1]; }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE T& B() noexcept { return data[2]; }

    HUH_NODISCARD HUH_CONSTEXPR_FORCE const T& R() const noexcept {
        return data[0];
    }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE const T& G() const noexcept {
        return data[1];
    }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE const T& B() const noexcept {
        return data[2];
    }

    HUH_NODISCARD HUH_CONSTEXPR_FORCE T* XYZ() noexcept { return data; }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE const T* XYZ() const noexcept {
        return data;
    }

    HUH_NODISCARD HUH_CONSTEXPR_FORCE T* RGB() noexcept { return data; }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE const T* RGB() const noexcept {
        return data;
    }

    HUH_NODISCARD HUH_CONSTEXPR_FORCE static std::size_t Size() { return 3; }

    HUH_NODISCARD HUH_CONSTEXPR_FORCE T& operator[](
        std::size_t index) noexcept {
        return data[index];
    }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE const T& operator[](
        std::size_t index) const noexcept {
        return data[index];
    }

    HUH_CONSTEXPR_FORCE Vector& operator+=(const Vector& rhs) noexcept {
        data[0] += rhs.data[0];
        data[1] += rhs.data[1];
        data[2] += rhs.data[2];
        return *this;
    }

    HUH_CONSTEXPR_FORCE friend Vector operator+(Vector lhs,
                                                const Vector& rhs) noexcept {
        lhs.data[0] += rhs.data[0];
        lhs.data[1] += rhs.data[1];
        lhs.data[2] += rhs.data[2];
        return lhs;
    }

    HUH_CONSTEXPR_FORCE Vector& operator-=(const Vector& rhs) noexcept {
        data[0] -= rhs.data[0];
        data[1] -= rhs.data[1];
        data[2] -= rhs.data[2];
        return *this;
    }

    HUH_CONSTEXPR_FORCE friend Vector operator-(Vector lhs,
                                                const Vector& rhs) noexcept {
        lhs.data[0] -= rhs.data[0];
        lhs.data[1] -= rhs.data[1];
        lhs.data[2] -= rhs.data[2];
        return lhs;
    }

    HUH_CONSTEXPR_FORCE Vector operator-() noexcept {
        return {-data[0], -data[1], -data[2]};
    }

    HUH_CONSTEXPR_FORCE Vector& operator*=(const Vector& rhs) noexcept {
        data[0] *= rhs.data[0];
        data[1] *= rhs.data[1];
        data[2] *= rhs.data[2];
        return *this;
    }

    HUH_CONSTEXPR_FORCE friend Vector operator*(Vector lhs,
                                                const Vector& rhs) noexcept {
        lhs.data[0] *= rhs.data[0];
        lhs.data[1] *= rhs.data[1];
        lhs.data[2] *= rhs.data[2];
        return lhs;
    }

    HUH_CONSTEXPR_FORCE Vector& operator*=(const T& rhs) noexcept {
        data[0] *= rhs;
        data[1] *= rhs;
        data[2] *= rhs;
        return *this;
    }

    HUH_CONSTEXPR_FORCE friend Vector operator*(Vector lhs,
                                                const T& rhs) noexcept {
        lhs.data[0] *= rhs;
        lhs.data[1] *= rhs;
        lhs.data[2] *= rhs;
        return lhs;
    }

    HUH_CONSTEXPR_FORCE Vector& operator/=(const Vector& rhs) noexcept {
        data[0] /= rhs.data[0];
        data[1] /= rhs.data[1];
        data[2] /= rhs.data[2];
        return *this;
    }

    HUH_CONSTEXPR_FORCE friend Vector operator/(Vector lhs,
                                                const Vector& rhs) noexcept {
        lhs.data[0] /= rhs.data[0];
        lhs.data[1] /= rhs.data[1];
        lhs.data[2] /= rhs.data[2];
        return lhs;
    }

    HUH_CONSTEXPR_FORCE Vector& operator/=(const T& rhs) noexcept {
        data[0] /= rhs;
        data[1] /= rhs;
        data[2] /= rhs;
        return *this;
    }

    HUH_CONSTEXPR_FORCE friend Vector operator/(Vector lhs,
                                                const T& rhs) noexcept {
        lhs.data[0] /= rhs;
        lhs.data[1] /= rhs;
        lhs.data[2] /= rhs;
        return lhs;
    }

};// namespace HUH