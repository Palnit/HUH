#pragma once

#include <HUH/definitions.h>
#include <HUH/types.h>
#include <cstdint>
#include <cstring>
#include <ostream>
#include <type_traits>

namespace HUH {
template<typename T, std::size_t N>
class Vector {
    static_assert(std::is_arithmetic_v<T>, "T must be arithmetic type");
    static_assert(N > 1, "N must be greater than 1");

public:
    T data[N];

    HUH_CONSTEXPR_FORCE Vector() noexcept : data{} {}
    HUH_CONSTEXPR_FORCE Vector(const Vector& other) noexcept = default;
    HUH_CONSTEXPR_FORCE Vector(Vector&& other) noexcept = default;
    HUH_CONSTEXPR_FORCE Vector& operator=(const Vector& other) noexcept =
        default;
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

    HUH_NODISCARD HUH_CONSTEXPR_FORCE T& operator[](
        std::size_t index) noexcept {
        return data[index];
    }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE const T& operator[](
        std::size_t index) const noexcept {
        return data[index];
    }

    HUH_CONSTEXPR_FORCE Vector& operator+=(const Vector& rhs) noexcept {
        for (std::size_t i = 0; i < N; i++) {
            data[i] += rhs.data[i];
        }
        return *this;
    }

    HUH_CONSTEXPR_FORCE friend Vector operator+(Vector lhs,
                                                const Vector& rhs) noexcept {
        for (std::size_t i = 0; i < N; i++) {
            lhs.data[i] += rhs.data[i];
        }
        return lhs;
    }

    HUH_CONSTEXPR_FORCE Vector& operator-=(const Vector& rhs) noexcept {
        for (std::size_t i = 0; i < N; i++) {
            data[i] -= rhs.data[i];
        }
        return *this;
    }

    HUH_CONSTEXPR_FORCE friend Vector operator-(Vector lhs,
                                                const Vector& rhs) noexcept {
        for (std::size_t i = 0; i < N; i++) {
            lhs.data[i] -= rhs.data[i];
        }
        return lhs;
    }

    HUH_CONSTEXPR_FORCE Vector& operator*=(const T& rhs) noexcept {
        for (std::size_t i = 0; i < N; i++) {
            data[i] *= rhs;
        }
        return *this;
    }

    HUH_CONSTEXPR_FORCE friend Vector operator*(Vector lhs,
                                                const T& rhs) noexcept {
        for (std::size_t i = 0; i < N; i++) {
            lhs.data[i] *= rhs;
        }
        return lhs;
    }

    HUH_CONSTEXPR_FORCE Vector& operator*=(const Vector& rhs) noexcept {
        for (std::size_t i = 0; i < N; i++) {
            data[i] *= rhs.data[i];
        }
        return *this;
    }

    HUH_CONSTEXPR_FORCE friend Vector operator*(Vector lhs,
                                                const Vector& rhs) noexcept {
        for (std::size_t i = 0; i < N; i++) {
            lhs.data[i] *= rhs.data[i];
        }
        return lhs;
    }

    HUH_NODISCARD HUH_CONSTEXPR_FORCE static std::size_t Size() { return N; }
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
constexpr const std::tuple_element_t<I, Vector<T, N>>& get(
    const Vector<T, N>& Vec) noexcept {
    return Vec[I];
}

template<std::size_t I, typename T, std::size_t N>
constexpr std::tuple_element_t<I, Vector<T, N>>& get(
    Vector<T, N>& Vec) noexcept {
    return Vec[I];
}

template<std::size_t I, typename T, std::size_t N>
constexpr const std::tuple_element_t<I, Vector<T, N>>&& get(
    const Vector<T, N>&& Vec) noexcept {
    return std::move(Vec[I]);
}

template<std::size_t I, typename T, std::size_t N>
constexpr std::tuple_element_t<I, Vector<T, N>>&& get(
    Vector<T, N>&& Vec) noexcept {
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

template<typename T>
class Vector<T, 2>;

template<typename T>
class Vector<T, 3>;

template<typename T>
class Vector<T, 4>;

template<typename T>
using Vector4 = Vector<T, 4>;

template<typename T>
using Vector3 = Vector<T, 3>;

template<typename T>
using Vector2 = Vector<T, 2>;

using Vector4f = Vector4<float>;
using Vector4d = Vector4<double>;
using Vector4i = Vector4<int>;

using Vector4ui = Vector4<unsigned int>;
using Vector4u8 = Vector4<Uint8>;
using Vector4u16 = Vector4<Uint16>;
using Vector4u32 = Vector4<Uint32>;
using Vector4u64 = Vector4<Uint64>;

using Vector4i8 = Vector4<Int8>;
using Vector4i16 = Vector4<Int16>;
using Vector4i32 = Vector4<Int32>;
using Vector4i64 = Vector4<Int64>;

using Vector3f = Vector3<float>;
using Vector3d = Vector3<double>;
using Vector3i = Vector3<int>;

using Vector3ui = Vector3<unsigned int>;
using Vector3u8 = Vector3<Uint8>;
using Vector3u16 = Vector3<Uint16>;
using Vector3u32 = Vector3<Uint32>;
using Vector3u64 = Vector3<Uint64>;

using Vector3i8 = Vector3<Int8>;
using Vector3i16 = Vector3<Int16>;
using Vector3i32 = Vector3<Int32>;
using Vector3i64 = Vector3<Int64>;

using Vector2f = Vector2<float>;
using Vector2d = Vector2<double>;
using Vector2i = Vector2<int>;

using Vector2ui = Vector2<unsigned int>;
using Vector2u8 = Vector2<Uint8>;
using Vector2u16 = Vector2<Uint16>;
using Vector2u32 = Vector2<Uint32>;
using Vector2u64 = Vector2<Uint64>;

using Vector2i8 = Vector2<Int8>;
using Vector2i16 = Vector2<Int16>;
using Vector2i32 = Vector2<Int32>;
using Vector2i64 = Vector2<Int64>;

}// namespace HUH

template<typename T, std::size_t N>
struct std::tuple_size<HUH::Vector<T, N>>
    : public integral_constant<std::size_t, N> {};

template<std::size_t I, typename T, std::size_t N>
struct std::tuple_element<I, HUH::Vector<T, N>> {
    using type = T;
};

#include <HUH/Math/vector/vector2.inl.h>
#include <HUH/Math/vector/vector3.inl.h>
#include <HUH/Math/vector/vector4.inl.h>
