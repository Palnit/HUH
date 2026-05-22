#pragma once

#include <HUH/definitions.h>
#include <HUH/Math/quaternion_functions.h>

namespace HUH {
template<typename T>
class Quaternion {
public:
    using ValueType = T;
    T data[4];

    HUH_CONSTEXPR_FORCE Quaternion() noexcept : data{} {}
    HUH_CONSTEXPR_FORCE Quaternion(const Quaternion& other) noexcept = default;
    HUH_CONSTEXPR_FORCE Quaternion(Quaternion&& other) noexcept = default;
    HUH_CONSTEXPR_FORCE Quaternion& operator=(const Quaternion& other) noexcept = default;
    HUH_CONSTEXPR_FORCE Quaternion& operator=(Quaternion&& other) noexcept = default;

    HUH_CONSTEXPR_FORCE Quaternion(const T& x, const T& y, const T& z, const T& w) noexcept : data{x, y, z, w} {}

    HUH_CONSTEXPR_FORCE Quaternion(const T& v) noexcept : data{v, v, v, v} {}

    template<std::size_t size, std::enable_if_t<size == 4, bool> = true>
    HUH_CONSTEXPR_FORCE Quaternion(const T (&t)[size]) noexcept : data{t[0], t[1], t[2], t[3]} {}

    HUH_NODISCARD HUH_CONSTEXPR_FORCE T& W() noexcept { return data[0]; }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE T& X() noexcept { return data[1]; }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE T& Y() noexcept { return data[2]; }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE T& Z() noexcept { return data[3]; }

    HUH_NODISCARD HUH_CONSTEXPR_FORCE const T& W() const noexcept { return data[0]; }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE const T& X() const noexcept { return data[1]; }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE const T& Y() const noexcept { return data[2]; }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE const T& Z() const noexcept { return data[3]; }

    HUH_NODISCARD HUH_CONSTEXPR_FORCE T& operator[](std::size_t index) noexcept { return data[index]; }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE const T& operator[](std::size_t index) const noexcept { return data[index]; }

    template<typename T2>
    HUH_CONSTEXPR_FORCE bool operator==(const Quaternion<T2>& rhs) {
        return data[0] == rhs.data[0] && data[1] == rhs.data[1] && data[2] == rhs.data[2] && data[3] == rhs.data[3];
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE bool operator!=(const Quaternion<T2>& rhs) {
        return !(*this == rhs);
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE Quaternion& operator+=(const T2& rhs) noexcept {
        data[0] += rhs;
        data[1] += rhs;
        data[2] += rhs;
        data[3] += rhs;
        return *this;
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE auto operator+(const T2& rhs) const noexcept {
        Quaternion<std::common_type_t<T, T2>> result{data[0] + rhs, data[1] + rhs, data[2] + rhs, data[3] + rhs};
        return result;
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE Quaternion& operator+=(const Quaternion<T2>& rhs) noexcept {
        data[0] += rhs.data[0];
        data[1] += rhs.data[1];
        data[2] += rhs.data[2];
        data[3] += rhs.data[3];
        return *this;
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE auto operator+(const Quaternion<T2>& rhs) const noexcept {
        Quaternion<std::common_type_t<T, T2>> result{data[0] + rhs.data[0], data[1] + rhs.data[1],
                                                     data[2] + rhs.data[2], data[3] + rhs.data[3]};
        return result;
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE Quaternion& operator-=(const T2& rhs) noexcept {
        data[0] -= rhs;
        data[1] -= rhs;
        data[2] -= rhs;
        data[3] -= rhs;
        return *this;
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE auto operator-(const T2& rhs) const noexcept {
        Quaternion<std::common_type_t<T, T2>> result{data[0] - rhs, data[1] - rhs, data[2] - rhs, data[3] - rhs};
        return result;
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE Quaternion& operator-=(const Quaternion<T2>& rhs) noexcept {
        data[0] -= rhs.data[0];
        data[1] -= rhs.data[1];
        data[2] -= rhs.data[2];
        data[3] -= rhs.data[3];
        return *this;
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE auto operator-(const Quaternion<T2>& rhs) const noexcept {
        Quaternion<std::common_type_t<T, T2>> result{data[0] - rhs.data[0], data[1] - rhs.data[1],
                                                     data[2] - rhs.data[2], data[3] - rhs.data[3]};
        return result;
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE Quaternion& operator*=(const T2& rhs) noexcept {
        data[0] *= rhs;
        data[1] *= rhs;
        data[2] *= rhs;
        data[3] *= rhs;
        return *this;
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE auto operator*(const T2& rhs) const noexcept {
        Quaternion<std::common_type_t<T, T2>> result{data[0] * rhs, data[1] * rhs, data[2] * rhs, data[3] * rhs};
        return result;
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE Quaternion& operator*=(const Quaternion<T2>& rhs) noexcept {
        Quaternion tmp;
        QuaternionMultiply(*this, rhs, tmp);
        *this = tmp;
        return *this;
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE auto operator*(const Quaternion<T2>& rhs) const noexcept {
        Quaternion result;
        QuaternionMultiply(*this, rhs, result);
        return result;
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE Quaternion& operator/=(const T2& rhs) noexcept {
        data[0] /= rhs;
        data[1] /= rhs;
        data[2] /= rhs;
        data[3] /= rhs;
        return *this;
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE auto operator/(const T2& rhs) const noexcept {
        Quaternion<std::common_type_t<T, T2>> result{data[0] / rhs, data[1] / rhs, data[2] / rhs, data[3] / rhs};
        return result;
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE Quaternion& operator/=(const Quaternion<T2>& rhs) noexcept {
        data[0] /= rhs.data[0];
        data[1] /= rhs.data[1];
        data[2] /= rhs.data[2];
        data[3] /= rhs.data[3];
        return *this;
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE auto operator/(const Quaternion<T2>& rhs) const noexcept {
        Quaternion<std::common_type_t<T, T2>> result{data[0] / rhs.data[0], data[1] / rhs.data[1],
                                                     data[2] / rhs.data[2], data[3] / rhs.data[3]};
        return result;
    }
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const Quaternion<T>& vec) {
    os << "[ " << vec[0];
    for (std::size_t i = 1; i < 4; i++) {
        os << ", " << vec[i];
    }
    os << "]";
    return os;
}

template<std::size_t I, typename T, std::size_t N>
constexpr const std::tuple_element_t<I, Quaternion<T>>& get(const Quaternion<T>& Vec) noexcept {
    return Vec[I];
}

template<std::size_t I, typename T, std::size_t N>
constexpr std::tuple_element_t<I, Quaternion<T>>& get(Quaternion<T>& Vec) noexcept {
    return Vec[I];
}

template<std::size_t I, typename T, std::size_t N>
constexpr const std::tuple_element_t<I, Quaternion<T>>&& get(const Quaternion<T>&& Vec) noexcept {
    return std::move(Vec[I]);
}

template<std::size_t I, typename T, std::size_t N>
constexpr std::tuple_element_t<I, Quaternion<T>>&& get(Quaternion<T>&& Vec) noexcept {
    return std::move(Vec[I]);
}

template<typename T, std::size_t N>
T* begin(Quaternion<T>& vec) {
    return vec.data;
}

template<typename T, std::size_t N>
T* end(Quaternion<T>& vec) {
    return vec.data + N;
}

}// namespace HUH

template<typename T>
struct std::tuple_size<HUH::Quaternion<T>> : public integral_constant<std::size_t, 4> {};

template<std::size_t I, typename T>
struct std::tuple_element<I, HUH::Quaternion<T>> {
    using type = T;
};

template<typename T>
struct std::formatter<HUH::Quaternion<T>> : std::formatter<std::string> {
    auto format(const HUH::Quaternion<T>& vec, format_context& ctx) const {
        std::stringstream ss;
        ss << "[ " << vec[0];
        for (std::size_t i = 1; i < 4; i++) {
            ss << ", " << vec[i];
        }
        ss << "]";
        return formatter<string>::format(ss.str(), ctx);
    }
};
