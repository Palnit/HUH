#pragma once

#include <HUH/Math/fwd.h>
#include <HUH/Math/vector_functions.h>
#include <type_traits>

#ifdef HUH_USE_SIMD
#include "HUH/Simd/shuffle.h"
#include "HUH/Simd/simd_functions.h"
#endif

namespace HUH {
template<typename T>
class Vector<T, 3> {

public:
    using ValueType = T;
    T data[3];

    HUH_CONSTEXPR_FORCE Vector() noexcept : data{} {}
    HUH_CONSTEXPR_FORCE Vector(const Vector& other) noexcept = default;
    HUH_CONSTEXPR_FORCE Vector(Vector&& other) noexcept = default;
    HUH_CONSTEXPR_FORCE Vector& operator=(const Vector& other) noexcept = default;
    HUH_CONSTEXPR_FORCE Vector& operator=(Vector&& other) noexcept = default;

    template<HUH::ConvertableFrom<T> T2>
    HUH_CONSTEXPR_FORCE Vector& operator=(Vector<T2, 3>&& other) noexcept {
        data[0] = static_cast<ValueType>(other[0]);
        data[1] = static_cast<ValueType>(other[1]);
        data[2] = static_cast<ValueType>(other[2]);
        return *this;
    }

    template<HUH::ConvertableFrom<T> T2>
    HUH_CONSTEXPR_FORCE Vector& operator=(const Vector<T2, 3>& other) noexcept {
        data[0] = static_cast<ValueType>(other[0]);
        data[1] = static_cast<ValueType>(other[1]);
        data[2] = static_cast<ValueType>(other[2]);
        return *this;
    }

    HUH_CONSTEXPR_FORCE Vector(const T& x, const T& y, const T& z) noexcept : data{x, y, z} {}
    HUH_CONSTEXPR_FORCE Vector(const T& v) noexcept : data{v, v, v} {}

    template<HUH::ConvertableFrom<T> T2>
    HUH_CONSTEXPR_FORCE Vector(const Vector<T2, 3>& other) noexcept
        : data{static_cast<ValueType>(other.data[0]), static_cast<ValueType>(other.data[1]),
               static_cast<ValueType>(other.data[2])} {}

    template<std::size_t size, std::enable_if_t<size == 3, bool> = true>
    HUH_CONSTEXPR_FORCE Vector(const T (&t)[size]) noexcept : data{t[0], t[1], t[2]} {}

    HUH_NODISCARD HUH_CONSTEXPR_FORCE T& X() noexcept { return data[0]; }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE T& Y() noexcept { return data[1]; }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE T& Z() noexcept { return data[2]; }

    HUH_NODISCARD HUH_CONSTEXPR_FORCE const T& X() const noexcept { return data[0]; }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE const T& Y() const noexcept { return data[1]; }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE const T& Z() const noexcept { return data[2]; }

    HUH_NODISCARD HUH_CONSTEXPR_FORCE T& R() noexcept { return data[0]; }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE T& G() noexcept { return data[1]; }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE T& B() noexcept { return data[2]; }

    HUH_NODISCARD HUH_CONSTEXPR_FORCE const T& R() const noexcept { return data[0]; }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE const T& G() const noexcept { return data[1]; }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE const T& B() const noexcept { return data[2]; }

    HUH_NODISCARD HUH_CONSTEXPR_FORCE T* XYZ() noexcept { return data; }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE const T* XYZ() const noexcept { return data; }

    HUH_NODISCARD HUH_CONSTEXPR_FORCE static std::size_t Size() { return 3; }

    HUH_NODISCARD HUH_CONSTEXPR_FORCE T& operator[](std::size_t index) noexcept { return data[index]; }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE const T& operator[](std::size_t index) const noexcept { return data[index]; }

    template<typename T2>
    HUH_CONSTEXPR_FORCE bool operator==(const Vector<T2, 4>& rhs) {
        return data[0] == rhs.data[0] && data[1] == rhs.data[1] && data[2] == rhs.data[2];
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE bool operator!=(const Vector<T2, 4>& rhs) {
        return !(*this == rhs);
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE Vector& operator+=(const T2& rhs) noexcept {
        data[0] += rhs;
        data[1] += rhs;
        data[2] += rhs;
        return *this;
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE auto operator+(const T2& rhs) const noexcept {
        Vector<std::common_type_t<T, T2>, 3> result{data[0] + rhs, data[1] + rhs, data[2] + rhs};
        return result;
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE Vector& operator+=(const Vector<T2, 3>& rhs) noexcept {
        data[0] += rhs.data[0];
        data[1] += rhs.data[1];
        data[2] += rhs.data[2];
        return *this;
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE auto operator+(const Vector<T2, 3>& rhs) const noexcept {
        Vector<std::common_type_t<T, T2>, 3> result{data[0] + rhs.data[0], data[1] + rhs.data[1],
                                                    data[2] + rhs.data[2]};
        return result;
    }

    HUH_CONSTEXPR_FORCE Vector& operator-() noexcept {
        data[0] = -data[0];
        data[1] = -data[1];
        data[2] = -data[2];
        return *this;
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE Vector& operator-=(const T2& rhs) noexcept {
        data[0] -= rhs;
        data[1] -= rhs;
        data[2] -= rhs;
        return *this;
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE auto operator-(const T2& rhs) const noexcept {
        Vector<std::common_type_t<T, T2>, 3> result{data[0] - rhs, data[1] - rhs, data[2] - rhs};
        return result;
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE Vector& operator-=(const Vector<T2, 3>& rhs) noexcept {
        data[0] -= rhs.data[0];
        data[1] -= rhs.data[1];
        data[2] -= rhs.data[2];
        return *this;
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE auto operator-(const Vector<T2, 3>& rhs) const noexcept {
        Vector<std::common_type_t<T, T2>, 3> result{data[0] - rhs.data[0], data[1] - rhs.data[1],
                                                    data[2] - rhs.data[2]};
        return result;
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE Vector& operator*=(const T2& rhs) noexcept {
        data[0] *= rhs;
        data[1] *= rhs;
        data[2] *= rhs;
        return *this;
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE auto operator*(const T2& rhs) const noexcept {
        Vector<std::common_type_t<T, T2>, 3> result{data[0] * rhs, data[1] * rhs, data[2] * rhs};
        return result;
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE Vector& operator*=(const Vector<T2, 3>& rhs) noexcept {
        data[0] *= rhs.data[0];
        data[1] *= rhs.data[1];
        data[2] *= rhs.data[2];
        return *this;
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE auto operator*(const Vector<T2, 3>& rhs) const noexcept {
        Vector<std::common_type_t<T, T2>, 3> result{data[0] * rhs.data[0], data[1] * rhs.data[1],
                                                    data[2] * rhs.data[2]};
        return result;
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE Vector& operator/=(const T2& rhs) noexcept {
        data[0] /= rhs;
        data[1] /= rhs;
        data[2] /= rhs;
        return *this;
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE auto operator/(const T2& rhs) const noexcept {
        Vector<std::common_type_t<T, T2>, 3> result{data[0] / rhs, data[1] / rhs, data[2] / rhs};
        return result;
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE Vector& operator/=(const Vector<T2, 3>& rhs) noexcept {
        data[0] /= rhs.data[0];
        data[1] /= rhs.data[1];
        data[2] /= rhs.data[2];
        return *this;
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE auto operator/(const Vector<T2, 3>& rhs) const noexcept {
        Vector<std::common_type_t<T, T2>, 3> result{data[0] / rhs.data[0], data[1] / rhs.data[1],
                                                    data[2] / rhs.data[2]};
        return result;
    }

    HUH_CONSTEXPR_FORCE T Dot(Vector rhs) noexcept { return HUH::Dot(*this, rhs); }

    HUH_CONSTEXPR_FORCE Vector& Normalize() noexcept {
        HUH::Normalize(*this);
        return *this;
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE Vector<std::common_type_t<T, T2>, 3> Cross(const Vector<T2, 3>& rhs) noexcept {
        Vector<std::common_type_t<T, T2>, 3> res;
        HUH::Cross(*this, rhs, res);
        return res;
    }

};// namespace HUH

#ifdef HUH_USE_SIMD

HUH_FORCE_INLINE float Dot(const HUH::Vector<float, 3>& lhs, const HUH::Vector<float, 3>& rhs) noexcept {
    Simd::Register<float, 4> prod(lhs.data[0], lhs.data[1], lhs.data[2], 0);
    prod *= Simd::Register<float, 4>(rhs.data[0], rhs.data[1], rhs.data[2], 0);
    return HUH::Simd::HSum(prod);
}

HUH_FORCE_INLINE void Normalize(HUH::Vector<float, 3>& vec) noexcept {
    Simd::Register<float, 4> Reg(vec.data[0], vec.data[1], vec.data[2], 0);
    Reg *= Reg;
    const Simd::Register<float, 4> Inv(HUH::Simd::InvSqrt(HUH::Simd::HSum(Reg)));
    Reg *= Inv;
    float tmp[4];
    Reg.Store(tmp);
    vec.data[0] = tmp[0];
    vec.data[1] = tmp[1];
    vec.data[2] = tmp[2];
}

#endif
}// namespace HUH