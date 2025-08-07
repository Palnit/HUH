#pragma once

#include <HUH/Math/fwd.h>
#include <HUH/Math/vector_functions.h>
#include <HUH/definitions.h>
#include <cstring>
#include <ostream>
#include <type_traits>

namespace HUH {
template<typename T, std::size_t N>
class Vector {
    static_assert(std::is_arithmetic_v<T>, "T must be arithmetic type");
    static_assert(N > 1, "N must be greater than 1");

public:
    using ValueType = T;
    T data[N];

    HUH_CONSTEXPR_FORCE Vector() noexcept : data{} {}
    HUH_CONSTEXPR_FORCE Vector(const Vector& other) noexcept = default;
    HUH_CONSTEXPR_FORCE Vector(Vector&& other) noexcept = default;
    HUH_CONSTEXPR_FORCE Vector& operator=(const Vector& other) noexcept = default;
    HUH_CONSTEXPR_FORCE Vector& operator=(Vector&& other) noexcept = default;

    HUH_CONSTEXPR_FORCE Vector(T v) noexcept : data{} {
        for (std::size_t i = 0; i < N; i++) {
            data[i] = v;
        }
    }

    HUH_CONSTEXPR_FORCE Vector& operator=(T v) noexcept {
        for (std::size_t i = 0; i < N; i++) {
            data[i] = v;
        }
        return *this;
    }

    template<typename... U,
             std::enable_if_t<1 + sizeof...(U) == N, bool> = true,
             std::enable_if_t<(std::is_same_v<T, U> && ...), bool> = true>
    HUH_CONSTEXPR_FORCE Vector(T t, U... u) noexcept : data(t, u...) {}

    template<std::size_t size, std::enable_if_t<size == N, bool> = true>
    HUH_CONSTEXPR_FORCE Vector(const T (&t)[size]) noexcept {
        std::memcpy(data, t, sizeof(T) * N);
    }

    HUH_NODISCARD HUH_CONSTEXPR_FORCE T& operator[](std::size_t index) noexcept { return data[index]; }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE const T& operator[](std::size_t index) const noexcept { return data[index]; }

    HUH_NODISCARD HUH_CONSTEXPR_FORCE static std::size_t Size() { return N; }

    template<typename T2>
    HUH_CONSTEXPR_FORCE bool operator==(const Vector<T2, N>& rhs) {
        for (std::size_t i = 0; i < N; i++) {
            if (data[i] != rhs[i]) { return false; }
        }
        return true;
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE bool operator!=(const Vector<T2, N>& rhs) {
        return !(*this == rhs);
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE Vector& operator+=(const T2& rhs) noexcept {
        for (std::size_t i = 0; i < N; i++) {
            data[i] += rhs;
        }
        return *this;
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE auto operator+(const T2& rhs) const noexcept {
        Vector<std::common_type_t<T, T2>, N> result;
        for (std::size_t i = 0; i < N; i++) {
            result.data[i] = data[i] + rhs;
        }
        return result;
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE Vector& operator+=(const Vector<T2, N>& rhs) noexcept {
        for (std::size_t i = 0; i < N; i++) {
            data[i] += rhs.data[i];
        }
        return *this;
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE auto operator+(const Vector<T2, N>& rhs) const noexcept {
        Vector<std::common_type_t<T, T2>, N> result;
        for (std::size_t i = 0; i < N; i++) {
            result.data[i] = data[i] + rhs.data[i];
        }
        return result;
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE Vector& operator-=(const T2& rhs) noexcept {
        for (std::size_t i = 0; i < N; i++) {
            data[i] -= rhs;
        }
        return *this;
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE auto operator-(const T2& rhs) const noexcept {
        Vector<std::common_type_t<T, T2>, N> result;
        for (std::size_t i = 0; i < N; i++) {
            result.data[i] = data[i] - rhs;
        }
        return result;
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE Vector& operator-=(const Vector<T2, N>& rhs) noexcept {
        for (std::size_t i = 0; i < N; i++) {
            data[i] -= rhs.data[i];
        }
        return *this;
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE auto operator-(const Vector<T2, N>& rhs) const noexcept {
        Vector<std::common_type_t<T, T2>, N> result;
        for (std::size_t i = 0; i < N; i++) {
            result.data[i] = data[i] - rhs.data[i];
        }
        return result;
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE Vector& operator*=(const T2& rhs) noexcept {
        for (std::size_t i = 0; i < N; i++) {
            data[i] *= rhs;
        }
        return *this;
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE auto operator*(const T2& rhs) const noexcept {
        Vector<std::common_type_t<T, T2>, N> result;
        for (std::size_t i = 0; i < N; i++) {
            result.data[i] = data[i] * rhs;
        }
        return result;
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE Vector& operator*=(const Vector<T2, N>& rhs) noexcept {
        for (std::size_t i = 0; i < N; i++) {
            data[i] *= rhs.data[i];
        }
        return *this;
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE Vector operator*(const Vector<T2, N>& rhs) const noexcept {
        Vector<std::common_type_t<T, T2>, N> result;
        for (std::size_t i = 0; i < N; i++) {
            result.data[i] = data[i] * rhs.data[i];
        }
        return result;
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE Vector& operator/=(const T2& rhs) noexcept {
        for (std::size_t i = 0; i < N; i++) {
            data[i] /= rhs;
        }
        return *this;
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE auto operator/(const T2& rhs) const noexcept {
        Vector<std::common_type_t<T, T2>, N> result;
        for (std::size_t i = 0; i < N; i++) {
            result.data[i] = data[i] / rhs;
        }
        return result;
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE Vector& operator/=(const Vector<T2, N>& rhs) noexcept {
        for (std::size_t i = 0; i < N; i++) {
            data[i] /= rhs.data[i];
        }
        return *this;
    }

    template<typename T2>
    HUH_CONSTEXPR_FORCE Vector operator/(const Vector<T2, N>& rhs) const noexcept {
        Vector<std::common_type_t<T, T2>, N> result;
        for (std::size_t i = 0; i < N; i++) {
            result.data[i] = data[i] / rhs.data[i];
        }
        return result;
    }

    HUH_CONSTEXPR_FORCE T Dot(const Vector& rhs) noexcept { return HUH::Dot(*this, rhs); }

    HUH_CONSTEXPR_FORCE Vector& Normalize() noexcept {
        HUH::Normalize(*this);
        return *this;
    }
};

template<typename T, std::size_t size>
std::ostream& operator<<(std::ostream& os, const Vector<T, size>& vec) {
    os << "[ " << vec[0];
    for (std::size_t i = 1; i < size; i++) {
        os << ", " << vec[i];
    }
    os << "]";
    return os;
}

template<std::size_t I, typename T, std::size_t N>
constexpr const std::tuple_element_t<I, Vector<T, N>>& get(const Vector<T, N>& Vec) noexcept {
    return Vec[I];
}

template<std::size_t I, typename T, std::size_t N>
constexpr std::tuple_element_t<I, Vector<T, N>>& get(Vector<T, N>& Vec) noexcept {
    return Vec[I];
}

template<std::size_t I, typename T, std::size_t N>
constexpr const std::tuple_element_t<I, Vector<T, N>>&& get(const Vector<T, N>&& Vec) noexcept {
    return std::move(Vec[I]);
}

template<std::size_t I, typename T, std::size_t N>
constexpr std::tuple_element_t<I, Vector<T, N>>&& get(Vector<T, N>&& Vec) noexcept {
    return std::move(Vec[I]);
}

template<typename T, std::size_t N>
T* begin(Vector<T, N>& vec) {
    return vec.data;
}

template<typename T, std::size_t N>
T* end(Vector<T, N>& vec) {
    return vec.data + N;
}

}// namespace HUH

template<typename T, std::size_t N>
struct std::tuple_size<HUH::Vector<T, N>> : public integral_constant<std::size_t, N> {};

template<std::size_t I, typename T, std::size_t N>
struct std::tuple_element<I, HUH::Vector<T, N>> {
    using type = T;
};

#include <HUH/Math/vector/vector2.inl.h>
#include <HUH/Math/vector/vector3.inl.h>
#include <HUH/Math/vector/vector4.inl.h>
