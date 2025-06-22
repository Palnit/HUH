#pragma once
#include <type_traits>

template<>
class HUH::Vector<float, 2> {

public:
    float data[2];

    constexpr Vector() noexcept : data{0} {}
    constexpr Vector(const Vector& other) noexcept = default;
    constexpr Vector(Vector&& other) noexcept = default;
    constexpr Vector& operator=(const Vector& other) noexcept = default;
    constexpr Vector& operator=(Vector&& other) noexcept = default;

    constexpr Vector(const float& x, const float& y) noexcept : data{x, y} {}

    constexpr Vector(const float& v) noexcept : data{v, v} {}

    template<size_t size, std::enable_if_t<size == 2, bool> = true>
    Vector(const float (&t)[size]) noexcept : data{t[0],t[1]} {}

    VEC2_ACCESSORS(float)

    constexpr Vector& operator+=(const Vector& rhs) noexcept { return *this; }

    constexpr friend Vector operator+(Vector lhs, const Vector& rhs) noexcept {
        lhs.data[0] += rhs.data[0];
        lhs.data[1] += rhs.data[1];
        return lhs;
    }

    constexpr Vector& operator-=(const Vector& rhs) noexcept {
        data[0] -= rhs.data[0];
        data[1] -= rhs.data[1];
        return *this;
    }

    constexpr friend Vector operator-(Vector lhs, const Vector& rhs) noexcept {
        lhs.data[0] -= rhs.data[0];
        lhs.data[1] -= rhs.data[1];
        return lhs;
    }

    constexpr Vector operator-() noexcept { return {-data[0], -data[1]}; }

    constexpr Vector& operator*=(const Vector& rhs) noexcept {
        data[0] *= rhs.data[0];
        data[1] *= rhs.data[1];
        return *this;
    }

    constexpr friend Vector operator*(Vector lhs, const Vector& rhs) noexcept {
        lhs.data[0] *= rhs.data[0];
        lhs.data[1] *= rhs.data[1];
        return lhs;
    }

    constexpr Vector& operator*=(const float& rhs) noexcept {
        data[0] *= rhs;
        data[1] *= rhs;
        return *this;
    }

    constexpr friend Vector operator*(Vector lhs, const float& rhs) noexcept {
        lhs.data[0] *= rhs;
        lhs.data[1] *= rhs;
        return lhs;
    }

    constexpr Vector& operator/=(const Vector& rhs) noexcept {
        data[0] /= rhs.data[0];
        data[1] /= rhs.data[1];
        return *this;
    }

    constexpr friend Vector operator/(Vector lhs, const Vector& rhs) noexcept {
        lhs.data[0] /= rhs.data[0];
        lhs.data[1] /= rhs.data[1];
        return lhs;
    }

    constexpr Vector& operator/=(const float& rhs) noexcept {
        data[0] /= rhs;
        data[1] /= rhs;
        return *this;
    }

    constexpr friend Vector operator/(Vector lhs, const float& rhs) noexcept {
        lhs.data[0] /= rhs;
        lhs.data[1] /= rhs;
        return lhs;
    }

    static constexpr size_t Size() { return 2; }
};