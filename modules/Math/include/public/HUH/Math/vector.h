#pragma once

#include <oneapi/tbb.h>
#include <type_traits>

#define VECTOR_PARALLEL_FOR(Body)                             \
    oneapi::tbb::parallel_for(static_cast<std::size_t>(0), N, \
                              [&](std::size_t i) { Body });

namespace HUH {
template<typename T, std::size_t N>
class Vector {

public:
    T data[N];

    constexpr Vector() noexcept : data{} {}
    constexpr Vector(const Vector& other) noexcept = default;
    constexpr Vector(Vector&& other) noexcept = default;
    constexpr Vector& operator=(const Vector& other) noexcept = default;
    constexpr Vector& operator=(Vector&& other) noexcept = default;

    Vector(T v) noexcept : data{} { VECTOR_PARALLEL_FOR(data[i] = v;) }

    Vector& operator=(T v) noexcept {
        VECTOR_PARALLEL_FOR(data[i] = v;)
        return *this;
    }

    template<typename... U,
             std::enable_if_t<1 + sizeof...(U) == N, bool> = true,
             std::enable_if_t<(std::is_same_v<T, U> && ...), bool> = true>
    Vector(T t, U... u) noexcept : data(t, u...) {}

    template<std::size_t size, std::enable_if_t<size == N, bool> = true>
    Vector(const T (&t)[size]) noexcept {
        std::memcpy(data, t, sizeof(T) * N);
    }

    [[nodiscard]] T& operator[](std::size_t index) noexcept {
        return data[index];
    }
    [[nodiscard]] const T& operator[](std::size_t index) const noexcept {
        return data[index];
    }

    Vector& operator+=(const Vector& rhs) noexcept {
        VECTOR_PARALLEL_FOR(data[i] += rhs.data[i];)
        return *this;
    }

    friend Vector operator+(Vector lhs, const Vector& rhs) noexcept {
        VECTOR_PARALLEL_FOR(lhs.data[i] += rhs.data[i];)
        return lhs;
    }

    Vector& operator-=(const Vector& rhs) noexcept {
        VECTOR_PARALLEL_FOR(data[i] -= rhs.data[i];)
        return *this;
    }

    friend Vector operator-(Vector lhs, const Vector& rhs) noexcept {
        VECTOR_PARALLEL_FOR(lhs.data[i] -= rhs.data[i];)
        return lhs;
    }

    Vector& operator*=(const T& rhs) noexcept {
        VECTOR_PARALLEL_FOR(data[i] *= rhs;)
        return *this;
    }

    friend Vector operator*(Vector lhs, const T& rhs) noexcept {
        VECTOR_PARALLEL_FOR(lhs.data[i] *= rhs;)
        return lhs;
    }

    Vector& operator*=(const Vector& rhs) noexcept {
        VECTOR_PARALLEL_FOR(data[i] *= rhs.data[i];)
        return *this;
    }

    friend Vector operator*(Vector lhs, const Vector& rhs) noexcept {
        VECTOR_PARALLEL_FOR(lhs.data[i] *= rhs.data[i];)
        return lhs;
    }

    [[nodiscard]] static constexpr std::size_t Size() { return N; }
};

template<typename T, std::size_t size>
std::ostream& operator<<(std::ostream& os, const Vector<T, size>& vec) {
    os << "[ " << vec[0];
    for (std::size_t i = 1; i < size; i++) { os << ", " << vec[i]; }
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
using Vector4u8 = Vector4<uint8_t>;
using Vector4u16 = Vector4<uint16_t>;
using Vector4u32 = Vector4<uint32_t>;
using Vector4u64 = Vector4<uint64_t>;

using Vector4i8 = Vector4<int8_t>;
using Vector4i16 = Vector4<int16_t>;
using Vector4i32 = Vector4<int32_t>;
using Vector4i64 = Vector4<int64_t>;

using Vector3f = Vector3<float>;
using Vector3d = Vector3<double>;
using Vector3i = Vector3<int>;

using Vector3ui = Vector3<unsigned int>;
using Vector3u8 = Vector3<uint8_t>;
using Vector3u16 = Vector3<uint16_t>;
using Vector3u32 = Vector3<uint32_t>;
using Vector3u64 = Vector3<uint64_t>;

using Vector3i8 = Vector3<int8_t>;
using Vector3i16 = Vector3<int16_t>;
using Vector3i32 = Vector3<int32_t>;
using Vector3i64 = Vector3<int64_t>;

using Vector2f = Vector2<float>;
using Vector2d = Vector2<double>;
using Vector2i = Vector2<int>;

using Vector2ui = Vector2<unsigned int>;
using Vector2u8 = Vector2<uint8_t>;
using Vector2u16 = Vector2<uint16_t>;
using Vector2u32 = Vector2<uint32_t>;
using Vector2u64 = Vector2<uint64_t>;

using Vector2i8 = Vector2<int8_t>;
using Vector2i16 = Vector2<int16_t>;
using Vector2i32 = Vector2<int32_t>;
using Vector2i64 = Vector2<int64_t>;

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
