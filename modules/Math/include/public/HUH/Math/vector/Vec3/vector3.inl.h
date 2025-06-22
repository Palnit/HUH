#pragma once

#include <type_traits>

template<typename T>
class HUH::Vector<T, 3> {

public:
    T data[3];

    constexpr Vector() noexcept : data{} {}
    constexpr Vector(const Vector& other) noexcept = default;
    constexpr Vector(Vector&& other) noexcept = default;
    constexpr Vector& operator=(const Vector& other) noexcept = default;
    constexpr Vector& operator=(Vector&& other) noexcept = default;

    constexpr Vector(const T& x, const T& y, const T& z) noexcept
        : data{x, y, z} {}
    constexpr Vector(const T& v) noexcept : data{v, v, v} {}

    template<size_t size, std::enable_if_t<size == 3, bool> = true>
    Vector(const T (&t)[size]) noexcept : data{t[0], t[1], t[2]} {}

    [[nodiscard]] constexpr T& X() noexcept { return data[0]; }
    [[nodiscard]] constexpr T& Y() noexcept { return data[1]; }
    [[nodiscard]] constexpr T& Z() noexcept { return data[2]; }

    [[nodiscard]] constexpr const T& X() const noexcept { return data[0]; }
    [[nodiscard]] constexpr const T& Y() const noexcept { return data[1]; }
    [[nodiscard]] constexpr const T& Z() const noexcept { return data[2]; }

    [[nodiscard]] constexpr T& R() noexcept { return data[0]; }
    [[nodiscard]] constexpr T& G() noexcept { return data[1]; }
    [[nodiscard]] constexpr T& B() noexcept { return data[2]; }

    [[nodiscard]] constexpr const T& R() const noexcept { return data[0]; }
    [[nodiscard]] constexpr const T& G() const noexcept { return data[1]; }
    [[nodiscard]] constexpr const T& B() const noexcept { return data[2]; }

    [[nodiscard]] constexpr T* XYZ() noexcept { return data; }
    [[nodiscard]] constexpr const T* XYZ() const noexcept { return data; }

    [[nodiscard]] constexpr T* RGB() noexcept { return data; }
    [[nodiscard]] constexpr const T* RGB() const noexcept { return data; }

    [[nodiscard]] T& operator[](size_t index) noexcept { return data[index]; }
    [[nodiscard]] const T& operator[](size_t index) const noexcept {
        return data[index];
    }

    constexpr Vector& operator+=(const Vector& rhs) noexcept {
        data[0] += rhs.data[0];
        data[1] += rhs.data[1];
        data[2] += rhs.data[2];
        return *this;
    }

    constexpr friend Vector operator+(Vector lhs, const Vector& rhs) noexcept {
        lhs.data[0] += rhs.data[0];
        lhs.data[1] += rhs.data[1];
        lhs.data[2] += rhs.data[2];
        return lhs;
    }

    constexpr Vector& operator-=(const Vector& rhs) noexcept {
        data[0] -= rhs.data[0];
        data[1] -= rhs.data[1];
        data[2] -= rhs.data[2];
        return *this;
    }

    constexpr friend Vector operator-(Vector lhs, const Vector& rhs) noexcept {
        lhs.data[0] -= rhs.data[0];
        lhs.data[1] -= rhs.data[1];
        lhs.data[2] -= rhs.data[2];
        return lhs;
    }

    constexpr Vector operator-() noexcept {
        return {-data[0], -data[1], -data[2]};
    }

    constexpr Vector& operator*=(const Vector& rhs) noexcept {
        data[0] *= rhs.data[0];
        data[1] *= rhs.data[1];
        data[2] *= rhs.data[2];
        return *this;
    }

    constexpr friend Vector operator*(Vector lhs, const Vector& rhs) noexcept {
        lhs.data[0] *= rhs.data[0];
        lhs.data[1] *= rhs.data[1];
        lhs.data[2] *= rhs.data[2];
        return lhs;
    }

    constexpr Vector& operator*=(const T& rhs) noexcept {
        data[0] *= rhs;
        data[1] *= rhs;
        data[2] *= rhs;
        return *this;
    }

    constexpr friend Vector operator*(Vector lhs, const T& rhs) noexcept {
        lhs.data[0] *= rhs;
        lhs.data[1] *= rhs;
        lhs.data[2] *= rhs;
        return lhs;
    }

    constexpr Vector& operator/=(const Vector& rhs) noexcept {
        data[0] /= rhs.data[0];
        data[1] /= rhs.data[1];
        data[2] /= rhs.data[2];
        return *this;
    }

    constexpr friend Vector operator/(Vector lhs, const Vector& rhs) noexcept {
        lhs.data[0] /= rhs.data[0];
        lhs.data[1] /= rhs.data[1];
        lhs.data[2] /= rhs.data[2];
        return lhs;
    }

    constexpr Vector& operator/=(const T& rhs) noexcept {
        data[0] /= rhs;
        data[1] /= rhs;
        data[2] /= rhs;
        return *this;
    }

    constexpr friend Vector operator/(Vector lhs, const T& rhs) noexcept {
        lhs.data[0] /= rhs;
        lhs.data[1] /= rhs;
        lhs.data[2] /= rhs;
        return lhs;
    }

    [[nodiscard]] static constexpr size_t Size() { return 3; }
};// namespace HUH