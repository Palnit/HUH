#pragma once

#include <HUH/Math/fwd.h>
#include <HUH/Math/vector_functions.h>
#include <type_traits>

#if HUH_USE_SIMD
#include "HUH/Simd/shuffle.h"
#include "HUH/Simd/simd_functions.h"
#endif

namespace HUH {
template<typename T>
class Vector<T, 4> {

public:
    using ValueType = T;
    T data[4];

    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Vector() noexcept : data{} {}
    HUH_CONSTEXPR_FORCE Vector(const Vector& other) noexcept = default;
    HUH_CONSTEXPR_FORCE Vector(Vector&& other) noexcept = default;
    HUH_CONSTEXPR_FORCE Vector& operator=(const Vector& other) noexcept = default;
    HUH_CONSTEXPR_FORCE Vector& operator=(Vector&& other) noexcept = default;

    template<HUH::ConvertableFrom<T> T2>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Vector& operator=(Vector<T2, 4>&& other) noexcept {
        data[0] = static_cast<ValueType>(other[0]);
        data[1] = static_cast<ValueType>(other[1]);
        data[2] = static_cast<ValueType>(other[2]);
        data[3] = static_cast<ValueType>(other[3]);
        return *this;
    }

    template<HUH::ConvertableFrom<T> T2>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Vector& operator=(const Vector<T2, 4>& other) noexcept {
        data[0] = static_cast<ValueType>(other[0]);
        data[1] = static_cast<ValueType>(other[1]);
        data[2] = static_cast<ValueType>(other[2]);
        data[3] = static_cast<ValueType>(other[3]);
        return *this;
    }

    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Vector(const T& x, const T& y, const T& z, const T& w) noexcept
        : data{x, y, z, w} {}

    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Vector(const T& v) noexcept : data{v, v, v, v} {}

    template<std::size_t size, std::enable_if_t<size == 4, bool> = true>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Vector(const T (&t)[size]) noexcept : data{t[0], t[1], t[2], t[3]} {}

    template<HUH::ConvertableFrom<T> T2, size_t N>
        requires(N > 4)
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Vector(const Vector<T2, N>& other) noexcept
        : data{static_cast<ValueType>(other.data[0]), static_cast<ValueType>(other.data[1]),
               static_cast<ValueType>(other.data[2])} {}

    template<HUH::ConvertableFrom<T> T2, size_t N>
        requires(N < 4)
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Vector(const Vector<T2, N>& other) noexcept : data{0} {
        for (int i = 0; i < N; ++i) {
            data[i] = other[i];
        }
    }

    HUH_NODISCARD HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE T& X() noexcept { return data[0]; }
    HUH_NODISCARD HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE T& Y() noexcept { return data[1]; }
    HUH_NODISCARD HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE T& Z() noexcept { return data[2]; }
    HUH_NODISCARD HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE T& W() noexcept { return data[3]; }

    HUH_NODISCARD HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE const T& X() const noexcept { return data[0]; }
    HUH_NODISCARD HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE const T& Y() const noexcept { return data[1]; }
    HUH_NODISCARD HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE const T& Z() const noexcept { return data[2]; }
    HUH_NODISCARD HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE const T& W() const noexcept { return data[3]; }

    HUH_NODISCARD HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE T& R() noexcept { return data[0]; }
    HUH_NODISCARD HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE T& G() noexcept { return data[1]; }
    HUH_NODISCARD HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE T& B() noexcept { return data[2]; }
    HUH_NODISCARD HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE T& A() noexcept { return data[3]; }

    HUH_NODISCARD HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE const T& R() const noexcept { return data[0]; }
    HUH_NODISCARD HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE const T& G() const noexcept { return data[1]; }
    HUH_NODISCARD HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE const T& B() const noexcept { return data[2]; }
    HUH_NODISCARD HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE const T& A() const noexcept { return data[3]; }

    HUH_NODISCARD HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE T* XYZW() noexcept { return data; }
    HUH_NODISCARD HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE const T* XYZW() const noexcept { return data; }

    HUH_NODISCARD HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE T* RGBA() noexcept { return data; }
    HUH_NODISCARD HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE const T* RGBA() const noexcept { return data; }

    HUH_NODISCARD HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE T& operator[](std::size_t index) noexcept {
        return data[index];
    }
    HUH_NODISCARD HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE const T& operator[](std::size_t index) const noexcept {
        return data[index];
    }
    HUH_NODISCARD HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE static std::size_t Size() { return 4; }

    template<typename T2>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE bool operator==(const Vector<T2, 4>& rhs) {
        return data[0] == rhs.data[0] && data[1] == rhs.data[1] && data[2] == rhs.data[2] && data[3] == rhs.data[3];
    }

    template<typename T2>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE bool operator!=(const Vector<T2, 4>& rhs) {
        return !(*this == rhs);
    }

    template<typename T2>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Vector& operator+=(const T2& rhs) noexcept {
        data[0] += rhs;
        data[1] += rhs;
        data[2] += rhs;
        data[3] += rhs;
        return *this;
    }

    template<typename T2>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE auto operator+(const T2& rhs) const noexcept {
        Vector<std::common_type_t<T, T2>, 4> result{data[0] + rhs, data[1] + rhs, data[2] + rhs, data[3] + rhs};
        return result;
    }

    template<typename T2>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Vector& operator+=(const Vector<T2, 4>& rhs) noexcept {
        data[0] += rhs.data[0];
        data[1] += rhs.data[1];
        data[2] += rhs.data[2];
        data[3] += rhs.data[3];
        return *this;
    }

    template<typename T2>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE auto operator+(const Vector<T2, 4>& rhs) const noexcept {
        Vector<std::common_type_t<T, T2>, 4> result{data[0] + rhs.data[0], data[1] + rhs.data[1], data[2] + rhs.data[2],
                                                    data[3] + rhs.data[3]};
        return result;
    }

    template<typename T2>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Vector& operator-=(const T2& rhs) noexcept {
        data[0] -= rhs;
        data[1] -= rhs;
        data[2] -= rhs;
        data[3] -= rhs;
        return *this;
    }

    template<typename T2>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE auto operator-(const T2& rhs) const noexcept {
        Vector<std::common_type_t<T, T2>, 4> result{data[0] - rhs, data[1] - rhs, data[2] - rhs, data[3] - rhs};
        return result;
    }

    template<typename T2>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Vector& operator-=(const Vector<T2, 4>& rhs) noexcept {
        data[0] -= rhs.data[0];
        data[1] -= rhs.data[1];
        data[2] -= rhs.data[2];
        data[3] -= rhs.data[3];
        return *this;
    }

    template<typename T2>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE auto operator-(const Vector<T2, 4>& rhs) const noexcept {
        Vector<std::common_type_t<T, T2>, 4> result{data[0] - rhs.data[0], data[1] - rhs.data[1], data[2] - rhs.data[2],
                                                    data[3] - rhs.data[3]};
        return result;
    }

    template<typename T2>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Vector& operator*=(const T2& rhs) noexcept {
        data[0] *= rhs;
        data[1] *= rhs;
        data[2] *= rhs;
        data[3] *= rhs;
        return *this;
    }

    template<typename T2>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE auto operator*(const T2& rhs) const noexcept {
        Vector<std::common_type_t<T, T2>, 4> result{data[0] * rhs, data[1] * rhs, data[2] * rhs, data[3] * rhs};
        return result;
    }

    template<typename T2>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Vector& operator*=(const Vector<T2, 4>& rhs) noexcept {
        data[0] *= rhs.data[0];
        data[1] *= rhs.data[1];
        data[2] *= rhs.data[2];
        data[3] *= rhs.data[3];
        return *this;
    }

    template<typename T2>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE auto operator*(const Vector<T2, 4>& rhs) const noexcept {
        Vector<std::common_type_t<T, T2>, 4> result{data[0] * rhs.data[0], data[1] * rhs.data[1], data[2] * rhs.data[2],
                                                    data[3] * rhs.data[3]};
        return result;
    }

    template<typename T2>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Vector& operator/=(const T2& rhs) noexcept {
        data[0] /= rhs;
        data[1] /= rhs;
        data[2] /= rhs;
        data[3] /= rhs;
        return *this;
    }

    template<typename T2>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE auto operator/(const T2& rhs) const noexcept {
        Vector<std::common_type_t<T, T2>, 4> result{data[0] / rhs, data[1] / rhs, data[2] / rhs, data[3] / rhs};
        return result;
    }

    template<typename T2>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Vector& operator/=(const Vector<T2, 4>& rhs) noexcept {
        data[0] /= rhs.data[0];
        data[1] /= rhs.data[1];
        data[2] /= rhs.data[2];
        data[3] /= rhs.data[3];
        return *this;
    }

    template<typename T2>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE auto operator/(const Vector<T2, 4>& rhs) const noexcept {
        Vector<std::common_type_t<T, T2>, 4> result{data[0] / rhs.data[0], data[1] / rhs.data[1], data[2] / rhs.data[2],
                                                    data[3] / rhs.data[3]};
        return result;
    }

    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE T Dot(Vector rhs) noexcept { return HUH::Dot(*this, rhs); }

    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Vector& Normalize() noexcept {
        HUH::Normalize(*this);
        return *this;
    }

    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE T Norm() noexcept { return HUH::Norm(*this); }
};

#if HUH_USE_SIMD
HUH_FORCE_INLINE float Dot(const HUH::Vector<float, 4>& lhs, const HUH::Vector<float, 4>& rhs) noexcept {
    Simd::Register<float, 4> prod(lhs.data);
    prod *= Simd::Register<float, 4>(rhs.data);
    return HUH::Simd::HSum(prod);
}

HUH_FORCE_INLINE void Normalize(HUH::Vector<float, 4>& vec) noexcept {
    Simd::Register<float, 4> Reg(vec.data);
    Reg *= Reg;
    const Simd::Register<float, 4> Inv(HUH::Simd::InvSqrt(HUH::Simd::HSum(Reg)));
    Reg *= Inv;
    Reg.Store(vec.data);
}

#endif

}// namespace HUH
