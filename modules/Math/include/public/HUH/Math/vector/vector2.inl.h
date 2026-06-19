#pragma once

#include <HUH/Math/fwd.h>
#include <type_traits>

template<typename T>
class HUH::Vector<T, 2> {

public:
    using ValueType = T;
    T data[2];

    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Vector() noexcept : data{0} {}
    HUH_CONSTEXPR_FORCE Vector(const Vector& other) noexcept = default;
    HUH_CONSTEXPR_FORCE Vector(Vector&& other) noexcept = default;
    HUH_CONSTEXPR_FORCE Vector& operator=(const Vector& other) noexcept = default;
    HUH_CONSTEXPR_FORCE Vector& operator=(Vector&& other) noexcept = default;

    template<HUH::ConvertableFrom<T> T2>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Vector& operator=(Vector<T2, 2>&& other) noexcept {
        data[0] = static_cast<ValueType>(other[0]);
        data[1] = static_cast<ValueType>(other[1]);
        return *this;
    }

    template<HUH::ConvertableFrom<T> T2>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Vector& operator=(const Vector<T2, 2>& other) noexcept {
        data[0] = static_cast<ValueType>(other[0]);
        data[1] = static_cast<ValueType>(other[1]);
        return *this;
    }

    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Vector(const T& x, const T& y) noexcept : data{x, y} {}

    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Vector(const T& v) noexcept : data{v, v} {}

    template<std::size_t size, std::enable_if_t<size == 2, bool> = true>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Vector(const T (&t)[size]) noexcept : data{t[0], t[1]} {}

    template<HUH::ConvertableFrom<T> T2, size_t N>
        requires(N > 2)
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Vector(const Vector<T2, N>& other) noexcept
        : data{static_cast<ValueType>(other.data[0]), static_cast<ValueType>(other.data[1]),
               static_cast<ValueType>(other.data[2])} {}

    HUH_NODISCARD HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE T& X() noexcept { return data[0]; }
    HUH_NODISCARD HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE T& Y() noexcept { return data[1]; }

    HUH_NODISCARD HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE const T& X() const noexcept { return data[0]; }
    HUH_NODISCARD HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE const T& Y() const noexcept { return data[1]; }

    HUH_NODISCARD HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE T& U() noexcept { return data[0]; }
    HUH_NODISCARD HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE T& V() noexcept { return data[1]; }

    HUH_NODISCARD HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE const T& U() const noexcept { return data[0]; }
    HUH_NODISCARD HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE const T& V() const noexcept { return data[1]; }

    HUH_NODISCARD HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE T& Width() noexcept { return data[0]; }
    HUH_NODISCARD HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE T& Height() noexcept { return data[1]; }

    HUH_NODISCARD HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE const T& Width() const noexcept { return data[0]; }
    HUH_NODISCARD HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE const T& Height() const noexcept { return data[1]; }

    HUH_NODISCARD HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE T* XY() noexcept { return data; }
    HUH_NODISCARD HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE const T* XY() const noexcept { return data; }

    HUH_NODISCARD HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE T* UV() noexcept { return data; }
    HUH_NODISCARD HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE const T* UV() const noexcept { return data; }

    HUH_NODISCARD HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE T& operator[](std::size_t index) noexcept {
        return data[index];
    }
    HUH_NODISCARD HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE const T& operator[](std::size_t index) const noexcept {
        return data[index];
    }
    HUH_NODISCARD HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE static std::size_t Size() { return 2; }

    template<typename T2>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE bool operator==(const Vector<T2, 2>& rhs) {
        return data[0] == rhs.data[0] && data[1] == rhs.data[1];
    }

    template<typename T2>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE bool operator!=(const Vector<T2, 2>& rhs) {
        return !(*this == rhs);
    }

    template<typename T2>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Vector& operator+=(const T2& rhs) noexcept {
        data[0] += rhs;
        data[1] += rhs;
        return *this;
    }

    template<typename T2>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE auto operator+(const T2& rhs) const noexcept {
        Vector<std::common_type_t<T, T2>, 2> result{data[0] + rhs, data[1] + rhs};
        return result;
    }

    template<typename T2>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Vector& operator+=(const Vector<T2, 2>& rhs) noexcept {
        data[0] += rhs.data[0];
        data[1] += rhs.data[1];
        return *this;
    }

    template<typename T2>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE auto operator+(const Vector<T2, 2>& rhs) const noexcept {
        Vector<std::common_type_t<T, T2>, 2> result{data[0] + rhs.data[0], data[1] + rhs.data[1]};
        return result;
    }

    template<typename T2>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Vector& operator-=(const T2& rhs) noexcept {
        data[0] -= rhs;
        data[1] -= rhs;
        return *this;
    }

    template<typename T2>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE auto operator-(const T2& rhs) const noexcept {
        Vector<std::common_type_t<T, T2>, 2> result{data[0] - rhs, data[1] - rhs};
        return result;
    }

    template<typename T2>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Vector& operator-=(const Vector<T2, 2>& rhs) noexcept {
        data[0] -= rhs.data[0];
        data[1] -= rhs.data[1];
        return *this;
    }

    template<typename T2>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE auto operator-(const Vector<T2, 2>& rhs) const noexcept {
        Vector<std::common_type_t<T, T2>, 2> result{data[0] - rhs.data[0], data[1] - rhs.data[1]};
        return result;
    }

    template<typename T2>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Vector& operator*=(const T2& rhs) noexcept {
        data[0] *= rhs;
        data[1] *= rhs;
        return *this;
    }

    template<typename T2>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE auto operator*(const T2& rhs) const noexcept {
        Vector<std::common_type_t<T, T2>, 2> result{data[0] * rhs, data[1] * rhs};
        return result;
    }

    template<typename T2>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Vector& operator*=(const Vector<T2, 2>& rhs) noexcept {
        data[0] *= rhs.data[0];
        data[1] *= rhs.data[1];
        return *this;
    }

    template<typename T2>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE auto operator*(const Vector<T2, 2>& rhs) const noexcept {
        Vector<std::common_type_t<T, T2>, 2> result{data[0] * rhs.data[0], data[1] * rhs.data[1]};
        return result;
    }

    template<typename T2>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Vector& operator/=(const T2& rhs) noexcept {
        data[0] /= rhs;
        data[1] /= rhs;
        return *this;
    }

    template<typename T2>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE auto operator/(const T2& rhs) const noexcept {
        Vector<std::common_type_t<T, T2>, 2> result{data[0] / rhs, data[1] / rhs};
        return result;
    }

    template<typename T2>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Vector& operator/=(const Vector<T2, 2>& rhs) noexcept {
        data[0] /= rhs.data[0];
        data[1] /= rhs.data[1];
        return *this;
    }

    template<typename T2>
    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE auto operator/(const Vector<T2, 2>& rhs) const noexcept {
        Vector<std::common_type_t<T, T2>, 2> result{data[0] / rhs.data[0], data[1] / rhs.data[1]};
        return result;
    }

    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE T Dot(Vector rhs) noexcept { return HUH::Dot(*this, rhs); }

    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Vector& Normalize() noexcept {
        HUH::Normalize(*this);
        return *this;
    }

    HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE T Norm() noexcept { return HUH::Norm(*this); }
};
