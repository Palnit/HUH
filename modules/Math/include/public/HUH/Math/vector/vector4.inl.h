#pragma once

#include <cmath>
#include <type_traits>

#ifdef HUH_USE_SIMD
#include "HUH/Simd/shuffle.h"
#include "HUH/Simd/simd_functions.h"
#endif

template<typename T>
class alignas(16) HUH::Vector<T, 4> {

public:
    alignas(16) T data[4];

    HUH_CONSTEXPR_FORCE Vector() noexcept : data{} {}
    HUH_CONSTEXPR_FORCE Vector(const Vector& other) noexcept = default;
    HUH_CONSTEXPR_FORCE Vector(Vector&& other) noexcept = default;
    HUH_CONSTEXPR_FORCE Vector& operator=(const Vector& other) noexcept =
        default;
    HUH_CONSTEXPR_FORCE Vector& operator=(Vector&& other) noexcept = default;

    HUH_CONSTEXPR_FORCE Vector(const T& x,
                               const T& y,
                               const T& z,
                               const T& w) noexcept
        : data{x, y, z, w} {}

    HUH_CONSTEXPR_FORCE Vector(const T& v) noexcept : data{v, v, v, v} {}

    template<std::size_t size, std::enable_if_t<size == 4, bool> = true>
    HUH_CONSTEXPR_FORCE Vector(const T (&t)[size]) noexcept
        : data{t[0], t[1], t[2], t[3]} {}

    HUH_NODISCARD HUH_CONSTEXPR_FORCE T& X() noexcept { return data[0]; }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE T& Y() noexcept { return data[1]; }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE T& Z() noexcept { return data[2]; }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE T& W() noexcept { return data[3]; }

    HUH_NODISCARD HUH_CONSTEXPR_FORCE const T& X() const noexcept {
        return data[0];
    }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE const T& Y() const noexcept {
        return data[1];
    }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE const T& Z() const noexcept {
        return data[2];
    }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE const T& W() const noexcept {
        return data[3];
    }

    HUH_NODISCARD HUH_CONSTEXPR_FORCE T& R() noexcept { return data[0]; }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE T& G() noexcept { return data[1]; }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE T& B() noexcept { return data[2]; }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE T& A() noexcept { return data[3]; }

    HUH_NODISCARD HUH_CONSTEXPR_FORCE const T& R() const noexcept {
        return data[0];
    }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE const T& G() const noexcept {
        return data[1];
    }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE const T& B() const noexcept {
        return data[2];
    }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE const T& A() const noexcept {
        return data[3];
    }

    HUH_NODISCARD HUH_CONSTEXPR_FORCE T* XYZW() noexcept { return data; }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE const T* XYZW() const noexcept {
        return data;
    }

    HUH_NODISCARD HUH_CONSTEXPR_FORCE T* RGBA() noexcept { return data; }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE const T* RGBA() const noexcept {
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
    HUH_NODISCARD HUH_CONSTEXPR_FORCE static std::size_t Size() { return 4; }

    HUH_CONSTEXPR_FORCE Vector& operator+=(const Vector& rhs) noexcept {
        data[0] += rhs.data[0];
        data[1] += rhs.data[1];
        data[2] += rhs.data[2];
        data[3] += rhs.data[3];
        return *this;
    }

    HUH_CONSTEXPR_FORCE friend Vector operator+(Vector lhs,
                                                const Vector& rhs) noexcept {
        lhs.data[0] += rhs.data[0];
        lhs.data[1] += rhs.data[1];
        lhs.data[2] += rhs.data[2];
        lhs.data[3] += rhs.data[3];
        return lhs;
    }

    HUH_CONSTEXPR_FORCE Vector& operator-=(const Vector& rhs) noexcept {
        data[0] -= rhs.data[0];
        data[1] -= rhs.data[1];
        data[2] -= rhs.data[2];
        data[3] -= rhs.data[3];
        return *this;
    }

    HUH_CONSTEXPR_FORCE friend Vector operator-(Vector lhs,
                                                const Vector& rhs) noexcept {
        lhs.data[0] -= rhs.data[0];
        lhs.data[1] -= rhs.data[1];
        lhs.data[2] -= rhs.data[2];
        lhs.data[3] -= rhs.data[3];
        return lhs;
    }

    HUH_CONSTEXPR_FORCE Vector operator-() noexcept {
        return {-data[0], -data[1], -data[2], -data[3]};
    }

    HUH_CONSTEXPR_FORCE Vector& operator*=(const Vector& rhs) noexcept {
        data[0] *= rhs.data[0];
        data[1] *= rhs.data[1];
        data[2] *= rhs.data[2];
        data[3] *= rhs.data[3];
        return *this;
    }

    HUH_CONSTEXPR_FORCE friend Vector operator*(Vector lhs,
                                                const Vector& rhs) noexcept {
        lhs.data[0] *= rhs.data[0];
        lhs.data[1] *= rhs.data[1];
        lhs.data[2] *= rhs.data[2];
        lhs.data[3] *= rhs.data[3];
        return lhs;
    }

    HUH_CONSTEXPR_FORCE Vector& operator*=(const T& rhs) noexcept {
        data[0] *= rhs;
        data[1] *= rhs;
        data[2] *= rhs;
        data[3] *= rhs;
        return *this;
    }

    HUH_CONSTEXPR_FORCE friend Vector operator*(Vector lhs,
                                                const T& rhs) noexcept {
        lhs.data[0] *= rhs;
        lhs.data[1] *= rhs;
        lhs.data[2] *= rhs;
        lhs.data[3] *= rhs;
        return lhs;
    }

    HUH_CONSTEXPR_FORCE Vector& operator/=(const Vector& rhs) noexcept {
        data[0] /= rhs.data[0];
        data[1] /= rhs.data[1];
        data[2] /= rhs.data[2];
        data[3] /= rhs.data[3];
        return *this;
    }

    HUH_CONSTEXPR_FORCE friend Vector operator/(Vector lhs,
                                                const Vector& rhs) noexcept {
        lhs.data[0] /= rhs.data[0];
        lhs.data[1] /= rhs.data[1];
        lhs.data[2] /= rhs.data[2];
        lhs.data[3] /= rhs.data[3];
        return lhs;
    }

    HUH_CONSTEXPR_FORCE Vector& operator/=(const T& rhs) noexcept {
        data[0] /= rhs;
        data[1] /= rhs;
        data[2] /= rhs;
        data[3] /= rhs;
        return *this;
    }

    HUH_CONSTEXPR_FORCE friend Vector operator/(Vector lhs,
                                                const T& rhs) noexcept {
        lhs.data[0] /= rhs;
        lhs.data[1] /= rhs;
        lhs.data[2] /= rhs;
        lhs.data[3] /= rhs;
        return lhs;
    }

    HUH_CONSTEXPR_FORCE T Dot(Vector rhs) noexcept {
#ifdef HUH_USE_SIMD
        Simd::Register<T, 4> prod(data);
        Simd::Register<T, 4> prod2(rhs.data);
        prod *= prod2;
        return HUH::Simd::HSum(prod);
#else
        return data[0] * rhs.data[0] + data[1] * rhs.data[1]
            + data[2] * rhs.data[2] + data[3] * rhs.data[3];
#endif
    }

    HUH_CONSTEXPR_FORCE Vector& Normalize() noexcept {
#ifdef HUH_USE_SIMD
        Simd::Register<T, 4> Reg(data);
        Reg *= Reg;
        const Simd::Register<T, 4> Inv(
            HUH::Simd::InvSqrt(HUH::Simd::HSum(Reg)));
        Reg = Reg * Inv;
        Reg.Store(data);
#else
        const T SquareSum = data[0] * data[0] + data[1] * data[1]
            + data[2] * data[2] + data[3] * data[3];
        const T sqrt = std::sqrtf(SquareSum);
        data[0] /= sqrt;
        data[1] /= sqrt;
        data[2] /= sqrt;
        data[3] /= sqrt;
#endif
        return *this;
    }
};
