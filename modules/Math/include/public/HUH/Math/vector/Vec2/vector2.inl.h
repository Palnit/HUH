#pragma once

#include <type_traits>

#define VEC2_ACCESSORS(type)                                                   \
    [[nodiscard]] constexpr type& X() noexcept { return data[0]; }             \
    [[nodiscard]] constexpr type& Y() noexcept { return data[1]; }             \
                                                                               \
    [[nodiscard]] constexpr const type& X() const noexcept { return data[0]; } \
    [[nodiscard]] constexpr const type& Y() const noexcept { return data[1]; } \
                                                                               \
    [[nodiscard]] constexpr type& U() noexcept { return data[0]; }             \
    [[nodiscard]] constexpr type& V() noexcept { return data[1]; }             \
                                                                               \
    [[nodiscard]] constexpr const type& U() const noexcept { return data[0]; } \
    [[nodiscard]] constexpr const type& V() const noexcept { return data[1]; } \
                                                                               \
    [[nodiscard]] constexpr type* XY() noexcept { return data; }               \
    [[nodiscard]] constexpr const type* XY() const noexcept { return data; }   \
                                                                               \
    [[nodiscard]] constexpr type* UV() noexcept { return data; }               \
    [[nodiscard]] constexpr const type* UV() const noexcept { return data; }   \
                                                                               \
    [[nodiscard]] type& operator[](size_t index) noexcept {                    \
        return data[index];                                                    \
    }                                                                          \
    [[nodiscard]] const type& operator[](size_t index) const noexcept {        \
        return data[index];                                                    \
    } \
    \
    static constexpr size_t Size() { return 2; } \


template<typename T>
class HUH::Vector<T, 2> {

public:
    T data[2];

    constexpr Vector() noexcept : data{0} {}
    constexpr Vector(const Vector& other) noexcept = default;
    constexpr Vector(Vector&& other) noexcept = default;
    constexpr Vector& operator=(const Vector& other) noexcept = default;
    constexpr Vector& operator=(Vector&& other) noexcept = default;

    constexpr Vector(const T& x, const T& y) noexcept : data{x, y} {}

    constexpr Vector(const T& v) noexcept : data{v, v} {}

    template<size_t size, std::enable_if_t<size == 2, bool> = true>
    Vector(const T (&t)[size]) noexcept : data{t[0], t[1]} {}

    VEC2_ACCESSORS(T)

    constexpr Vector& operator+=(const Vector& rhs) noexcept {
        data[0] += rhs.data[0];
        data[1] += rhs.data[1];
        return *this;
    }

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

    constexpr Vector& operator*=(const T& rhs) noexcept {
        data[0] *= rhs;
        data[1] *= rhs;
        return *this;
    }

    constexpr friend Vector operator*(Vector lhs, const T& rhs) noexcept {
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

    constexpr Vector& operator/=(const T& rhs) noexcept {
        data[0] /= rhs;
        data[1] /= rhs;
        return *this;
    }

    constexpr friend Vector operator/(Vector lhs, const T& rhs) noexcept {
        lhs.data[0] /= rhs;
        lhs.data[1] /= rhs;
        return lhs;
    }

};

#ifdef HUH_USE_SIMD
#include <HUH/Math/vector/Vec2/Simd/vector2_float.inl.h>
#endif
