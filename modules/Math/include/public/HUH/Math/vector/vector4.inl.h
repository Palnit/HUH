#pragma once

#include <type_traits>

#ifdef HUH_USE_SIMD
#include "HUH/Simd/hsum.h"
#include "HUH/Simd/shuffle.h"
#endif

template<typename T>
class HUH::Vector<T, 4> {

public:
    T data[4];

    constexpr Vector() noexcept : data{} {}
    constexpr Vector(const Vector& other) noexcept = default;
    constexpr Vector(Vector&& other) noexcept = default;
    constexpr Vector& operator=(const Vector& other) noexcept = default;
    constexpr Vector& operator=(Vector&& other) noexcept = default;

    constexpr Vector(const T& x, const T& y, const T& z, const T& w) noexcept
        : data{x, y, z, w} {}

    constexpr Vector(const T& v) noexcept : data{v, v, v, v} {}

    template<std::size_t size, std::enable_if_t<size == 4, bool> = true>
    Vector(const T (&t)[size]) noexcept : data{t[0], t[1], t[2], t[3]} {}

    [[nodiscard]] constexpr T& X() noexcept { return data[0]; }
    [[nodiscard]] constexpr T& Y() noexcept { return data[1]; }
    [[nodiscard]] constexpr T& Z() noexcept { return data[2]; }
    [[nodiscard]] constexpr T& W() noexcept { return data[3]; }

    [[nodiscard]] constexpr const T& X() const noexcept { return data[0]; }
    [[nodiscard]] constexpr const T& Y() const noexcept { return data[1]; }
    [[nodiscard]] constexpr const T& Z() const noexcept { return data[2]; }
    [[nodiscard]] constexpr const T& W() const noexcept { return data[3]; }

    [[nodiscard]] constexpr T& R() noexcept { return data[0]; }
    [[nodiscard]] constexpr T& G() noexcept { return data[1]; }
    [[nodiscard]] constexpr T& B() noexcept { return data[2]; }
    [[nodiscard]] constexpr T& A() noexcept { return data[3]; }

    [[nodiscard]] constexpr const T& R() const noexcept { return data[0]; }
    [[nodiscard]] constexpr const T& G() const noexcept { return data[1]; }
    [[nodiscard]] constexpr const T& B() const noexcept { return data[2]; }
    [[nodiscard]] constexpr const T& A() const noexcept { return data[3]; }

    [[nodiscard]] constexpr T* XYZW() noexcept { return data; }
    [[nodiscard]] constexpr const T* XYZW() const noexcept { return data; }

    [[nodiscard]] constexpr T* RGBA() noexcept { return data; }
    [[nodiscard]] constexpr const T* RGBA() const noexcept { return data; }

    [[nodiscard]] constexpr T& operator[](std::size_t index) noexcept {
        return data[index];
    }
    [[nodiscard]] constexpr const T& operator[](
        std::size_t index) const noexcept {
        return data[index];
    }
    [[nodiscard]] static constexpr std::size_t Size() { return 4; }

    constexpr Vector& operator+=(const Vector& rhs) noexcept {
        data[0] += rhs.data[0];
        data[1] += rhs.data[1];
        data[2] += rhs.data[2];
        data[3] += rhs.data[3];
        return *this;
    }

    constexpr friend Vector operator+(Vector lhs, const Vector& rhs) noexcept {
        lhs.data[0] += rhs.data[0];
        lhs.data[1] += rhs.data[1];
        lhs.data[2] += rhs.data[2];
        lhs.data[3] += rhs.data[3];
        return lhs;
    }

    constexpr Vector& operator-=(const Vector& rhs) noexcept {
        data[0] -= rhs.data[0];
        data[1] -= rhs.data[1];
        data[2] -= rhs.data[2];
        data[3] -= rhs.data[3];
        return *this;
    }

    constexpr friend Vector operator-(Vector lhs, const Vector& rhs) noexcept {
        lhs.data[0] -= rhs.data[0];
        lhs.data[1] -= rhs.data[1];
        lhs.data[2] -= rhs.data[2];
        lhs.data[3] -= rhs.data[3];
        return lhs;
    }

    constexpr Vector operator-() noexcept {
        return {-data[0], -data[1], -data[2], -data[3]};
    }

    constexpr Vector& operator*=(const Vector& rhs) noexcept {
        data[0] *= rhs.data[0];
        data[1] *= rhs.data[1];
        data[2] *= rhs.data[2];
        data[3] *= rhs.data[3];
        return *this;
    }

    constexpr friend Vector operator*(Vector lhs, const Vector& rhs) noexcept {
        lhs.data[0] *= rhs.data[0];
        lhs.data[1] *= rhs.data[1];
        lhs.data[2] *= rhs.data[2];
        lhs.data[3] *= rhs.data[3];
        return lhs;
    }

    constexpr Vector& operator*=(const T& rhs) noexcept {
        data[0] *= rhs;
        data[1] *= rhs;
        data[2] *= rhs;
        data[3] *= rhs;
        return *this;
    }

    constexpr friend Vector operator*(Vector lhs, const T& rhs) noexcept {
        lhs.data[0] *= rhs;
        lhs.data[1] *= rhs;
        lhs.data[2] *= rhs;
        lhs.data[3] *= rhs;
        return lhs;
    }

    constexpr Vector& operator/=(const Vector& rhs) noexcept {
        data[0] /= rhs.data[0];
        data[1] /= rhs.data[1];
        data[2] /= rhs.data[2];
        data[3] /= rhs.data[3];
        return *this;
    }

    constexpr friend Vector operator/(Vector lhs, const Vector& rhs) noexcept {
        lhs.data[0] /= rhs.data[0];
        lhs.data[1] /= rhs.data[1];
        lhs.data[2] /= rhs.data[2];
        lhs.data[3] /= rhs.data[3];
        return lhs;
    }

    constexpr Vector& operator/=(const T& rhs) noexcept {
        data[0] /= rhs;
        data[1] /= rhs;
        data[2] /= rhs;
        data[3] /= rhs;
        return *this;
    }

    constexpr friend Vector operator/(Vector lhs, const T& rhs) noexcept {
        lhs.data[0] /= rhs;
        lhs.data[1] /= rhs;
        lhs.data[2] /= rhs;
        lhs.data[3] /= rhs;
        return lhs;
    }

    constexpr T Dot(Vector rhs) noexcept {
#ifdef HUH_USE_SIMD
        Simd::Register<float, 4> prod(data);
        prod = prod * Simd::Register<float, 4>(rhs.data);
        return HUH::Simd::HSum(prod);
#else
        return data[0] * rhs.data[0] + data[1] * rhs.data[1]
            + data[2] * rhs.data[2] + data[3] * rhs.data[3];
#endif
    }
};
