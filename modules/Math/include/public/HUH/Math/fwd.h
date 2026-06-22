#pragma once

#include <HUH/concepts.h>
#include <HUH/types.h>

#include <type_traits>

namespace HUH {
template<typename T, std::size_t N>
class Vector;

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

template<typename T, std::size_t R, std::size_t C>
class Matrix;

template<typename T>
class Matrix<T, 4, 4>;

// template<typename T>
// class Matrix<T, 3, 3>;
//
// template<typename T>
// class Matrix<T, 2, 2>;
//
template<typename T>
using Matrix4x4 = Matrix<T, 4, 4>;

template<typename T>
using Matrix3x3 = Matrix<T, 3, 3>;
//
// template<typename T>
// using Matrix2x2 = Matrix<T, 2, 2>;
//
using Matrix4x4f = Matrix4x4<float>;
using Matrix4x4d = Matrix4x4<double>;
using Matrix4x4i = Matrix4x4<int>;
using Matrix4x4ui = Matrix4x4<unsigned int>;

using Matrix3x3f = Matrix3x3<float>;
using Matrix3x3d = Matrix3x3<double>;
using Matrix3x3i = Matrix3x3<int>;
using Matrix3x3ui = Matrix3x3<unsigned int>;

// template<typename>
// struct IsVector : public std::false_type {};
//
// template<typename T, std::size_t N>
// struct IsVector<HUH::Vector<T, N>> : public std::true_type {};

template<FloatingPoint T>
class Quaternion;

using Quaternionf = Quaternion<float>;
using Quaterniond = Quaternion<double>;

}// namespace HUH