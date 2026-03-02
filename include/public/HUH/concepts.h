#pragma once

#include <functional>
#include <type_traits>

namespace HUH {

// Convenience concepts avoids the use of enable_if

template<typename T, typename U>
concept Derived = std::is_base_of_v<U, T>;

template<typename T, typename U>
concept Convertable = std::is_convertible_v<T, U>;

template<typename T>
concept Integral = std::is_integral_v<T>;

template<typename T>
concept FloatingPoint = std::is_floating_point_v<T>;

template<typename T>
concept Aritmatic = std::is_arithmetic_v<T>;

template<typename T>
concept Class = std::is_class_v<T>;

template<typename T, typename U>
concept Same = std::is_same_v<T, U>;

template<typename T>
concept DefaultConstructable = std::is_default_constructible_v<T>;

template<typename T, typename U>
concept Constructable = std::is_constructible_v<U, T>;

template<typename T, typename U>
concept Assignable = std::is_assignable_v<U, T>;

template<typename T>
concept Comparable = std::equality_comparable<T>;

template<typename T, typename U>
concept ComparableWith = std::equality_comparable_with<T, U>;

template<typename T>
concept Const = std::is_const_v<T>;

template<typename T>
concept Reference = std::is_reference_v<T>;

template<typename T>
concept RValueReference = std::is_rvalue_reference_v<T>;

template<typename T>
concept Pointer = std::is_pointer_v<T>;

template<typename T>
concept Enum = std::is_enum_v<T>;

// Concepts for niche but helpful things

template<typename T, typename... Type>
concept HasSubscribe = requires(T t, Type... v) { t.operator[](v...); };

template<typename T, typename U>
concept HasLessThan = requires(T a, U b) { b < a; };

template<typename T, typename U>
concept HasLessThanEqual = requires(T a, U b) { b <= a; };

template<typename T, typename U>
concept HasGreaterThan = requires(T a, U b) { b > a; };

template<typename T, typename U>
concept HasGreaterThanEqual = requires(T a, U b) { b > a; };

template<typename T, typename U>
concept HasAddition = requires(T a, U b) { b + a; };

template<typename T, typename U>
concept HasSubtraction = requires(T a, U b) { b - a; };

template<typename T, typename U>
concept HasMultiplication = requires(T a, U b) { b * a; };

template<typename T, typename U>
concept HasDivision = requires(T a, U b) { b / a; };

}// namespace HUH