#pragma once

#include "HUH/types.h"
#include "tuple.h"

#include <HUH/definitions.h>
#include <functional>

#include <type_traits>
#include <utility>

namespace HUH {

template<typename T1, typename T2>
class Pair;

template<typename T>
struct is_pair : std::false_type {};

template<typename T1, typename T2>
struct is_pair<Pair<T1, T2>> : std::true_type {};

template<typename T>
concept PairType = is_pair<std::remove_cvref_t<T>>::value;

template<typename T1, typename T2>
class Pair {
public:
    T1 first;
    T2 second;

    Pair() = default;
    Pair(Pair&&) = default;
    Pair(const Pair&) = default;
    Pair& operator=(Pair&&) = default;
    Pair& operator=(const Pair&) = default;

    Pair& operator=(const Pair& other)
        requires requires(Pair& lhs, const Pair& rhs) {
            { HUH::Assignable<decltype(lhs.first), decltype(rhs.first)> };
            { HUH::Assignable<decltype(lhs.second), decltype(rhs.second)> };
        }
    {
        Assign(*this, other);
        return *this;
    }

    Pair& operator=(Pair&& other) noexcept
        requires requires(Pair& lhs, Pair&& rhs) {
            { HUH::Assignable<decltype(lhs.first), decltype(std::move(rhs.first))> };
            { HUH::Assignable<decltype(lhs.second), decltype(std::move(rhs.second))> };
        }
    {
        Assign(*this, std::move(other));
        return *this;
    }

    Pair(T1 first, T2 second) : first(first), second(second) {}

private:
    template<Uint64 Index, Uint64 Step, typename... HelperArgs>
    struct ElementTypeHelper {
        using Type = void;
    };

    template<Uint64 Index, Uint64 Step, typename T, typename... HelperArgs>
    struct ElementTypeHelper<Index, Step, T, HelperArgs...> {
        using Type = ElementTypeHelper<Index, Step + 1, HelperArgs...>::Type;
    };

    template<Uint64 Index, Uint64 Step, typename T, typename... HelperArgs>
        requires(Index == Step)
    struct ElementTypeHelper<Index, Step, T, HelperArgs...> {
        using Type = T;
    };

    template<typename LhsTuple, typename RhsTuple>
    static HUH_FORCE_INLINE void Assign(LhsTuple& lhs, RhsTuple&& rhs) {
        lhs.template Get<0>() = std::forward<RhsTuple>(rhs).template Get<0>();
        lhs.template Get<1>() = std::forward<RhsTuple>(rhs).template Get<1>();
    }

public:
    template<Uint64 Index>
        requires(Index < 2)
    using ElementType = ElementTypeHelper<Index, 0, T1, T2>::Type;

    template<PairType OtherPair>
    explicit Pair(OtherPair&& other)
        : first(std::forward<OtherPair>(other).first),
          second(std::forward<OtherPair>(other).second) {}

    template<std::size_t Index>
        requires(Index < 2)
    HUH_FORCE_INLINE constexpr decltype(auto) Get() & {
        if constexpr (Index == 0) {
            return (first);
        } else {
            return (second);
        }
    }

    template<HUH::Assignable<T1> Args1, HUH::Assignable<T2> Args2>
        requires(!HUH::Same<Pair, Pair<Args1, Args2>>)
    HUH_FORCE_INLINE Pair& operator=(const Pair<Args1, Args2>& other) {
        Assign(*this, other);
        return *this;
    }

    template<HUH::Assignable<T1> Args1, HUH::Assignable<T2> Args2>
        requires(!HUH::Same<Pair, Pair<Args1, Args2>>)
    HUH_FORCE_INLINE Pair& operator=(Pair<Args1, Args2>&& other) {
        Assign(*this, std::move(other));
        return *this;
    }

    template<HUH::ComparableWith<T1> Args1, HUH::ComparableWith<T2> Args2>
    HUH_FORCE_INLINE bool operator==(const Pair<Args1, Args2>& other) const {
        return this->Get<0>() == other.template Get<0>() && this->Get<1>() == other.template Get<1>();
    }

    template<HUH::ComparableWith<T1> Args1, HUH::ComparableWith<T2> Args2>
    HUH_FORCE_INLINE bool operator!=(const Pair<Args1, Args2>& other) const {
        return !(*this == other);
    }

    template<HUH::HasLessThan<T1> Args1, HUH::HasLessThan<T2> Args2>
    HUH_FORCE_INLINE bool operator<(const Pair<Args1, Args2>& other) const {
        return this->Get<0>() < other.template Get<0>() && this->Get<1>() < other.template Get<1>();
    }

    template<HUH::HasLessThanEqual<T1> Args1, HUH::HasLessThanEqual<T2> Args2>
    HUH_FORCE_INLINE bool operator<=(const Pair<Args1, Args2>& other) const {
        return this->Get<0>() <= other.template Get<0>() && this->Get<1>() <= other.template Get<1>();
    }

    template<HUH::HasGreaterThan<T1> Args1, HUH::HasGreaterThan<T2> Args2>
    HUH_FORCE_INLINE bool operator>(const Pair<Args1, Args2>& other) const {
        return this->Get<0>() > other.template Get<0>() && this->Get<1>() > other.template Get<1>();
    }

    template<HUH::HasGreaterThanEqual<T1> Args1, HUH::HasGreaterThanEqual<T2> Args2>
    HUH_FORCE_INLINE bool operator>=(const Pair<Args1, Args2>& other) const {
        return this->Get<0>() >= other.template Get<0>() && this->Get<1>() >= other.template Get<1>();
    }

    template<std::size_t Index>
        requires(Index < 2)
    HUH_FORCE_INLINE constexpr decltype(auto) Get() const& {
        if constexpr (Index == 0) {
            return (first);
        } else {
            return (second);
        }
    }

    template<std::size_t Index>
        requires(Index < 2)
    HUH_FORCE_INLINE constexpr decltype(auto) Get() volatile& {
        if constexpr (Index == 0) {
            return (first);
        } else {
            return (second);
        }
    }

    template<std::size_t Index>
        requires(Index < 2)
    HUH_FORCE_INLINE constexpr decltype(auto) Get() const volatile& {
        if constexpr (Index == 0) {
            return (first);
        } else {
            return (second);
        }
    }

    template<std::size_t Index>
        requires(Index < 2)
    HUH_FORCE_INLINE constexpr decltype(auto) Get() && {
        if constexpr (Index == 0) {
            return (static_cast<Pair&&>(*this).first);
        } else {
            return (static_cast<Pair&&>(*this).second);
        }
    }

    template<std::size_t Index>
        requires(Index < 2)
    HUH_FORCE_INLINE constexpr decltype(auto) Get() const&& {
        if constexpr (Index == 0) {
            return (static_cast<const Pair&&>(*this).first);
        } else {
            return (static_cast<const Pair&&>(*this).second);
        }
    }

    template<std::size_t Index>
        requires(Index < 2)
    HUH_FORCE_INLINE constexpr decltype(auto) Get() volatile&& {
        if constexpr (Index == 0) {
            return (static_cast<volatile Pair&&>(*this).first);
        } else {
            return (static_cast<volatile Pair&&>(*this).second);
        }
    }

    template<std::size_t Index>
        requires(Index < 2)
    HUH_FORCE_INLINE constexpr decltype(auto) Get() const volatile&& {
        if constexpr (Index == 0) {
            return (static_cast<const volatile Pair&&>(*this).first);
        } else {
            return (static_cast<const volatile Pair&&>(*this).second);
        }
    }

    /// Apply's the pair's elements to the end of a function call
    /// @param func The function to call
    /// @param args The arguments of the function before pair elements
    /// @return The return value of the function
    template<typename Func, typename... Args>
    HUH_FORCE_INLINE decltype(auto) ApplyAfter(Func&& func, Args... args) & {
        return std::invoke(func, std::forward<Args>(args)..., static_cast<Pair&>(*this).first,
                           static_cast<Pair&>(*this).second);
    }
    template<typename Func, typename... Args>
    HUH_FORCE_INLINE decltype(auto) ApplyAfter(Func&& func, Args... args) const& {
        return std::invoke(func, std::forward<Args>(args)..., static_cast<const Pair&>(*this).first,
                           static_cast<const Pair&>(*this).second);
    }
    template<typename Func, typename... Args>
    HUH_FORCE_INLINE decltype(auto) ApplyAfter(Func&& func, Args... args) volatile& {
        return std::invoke(func, std::forward<Args>(args)..., static_cast<volatile Pair&>(*this).first,
                           static_cast<volatile Pair&>(*this).second);
    }
    template<typename Func, typename... Args>
    HUH_FORCE_INLINE decltype(auto) ApplyAfter(Func&& func, Args... args) const volatile& {
        return std::invoke(func, std::forward<Args>(args)..., static_cast<const volatile Pair&>(*this).first,
                           static_cast<const volatile Pair&>(*this).second);
    }
    template<typename Func, typename... Args>
    HUH_FORCE_INLINE decltype(auto) ApplyAfter(Func&& func, Args... args) && {
        return std::invoke(func, std::forward<Args>(args)..., static_cast<Pair&&>(*this).first,
                           static_cast<Pair&&>(*this).second);
    }
    template<typename Func, typename... Args>
    HUH_FORCE_INLINE decltype(auto) ApplyAfter(Func&& func, Args... args) const&& {
        return std::invoke(func, std::forward<Args>(args)..., static_cast<const Pair&&>(*this).first,
                           static_cast<const Pair&&>(*this).second);
    }
    template<typename Func, typename... Args>
    HUH_FORCE_INLINE decltype(auto) ApplyAfter(Func&& func, Args... args) volatile&& {
        return std::invoke(func, std::forward<Args>(args)..., static_cast<volatile Pair&&>(*this).first,
                           static_cast<volatile Pair&&>(*this).second);
    }
    template<typename Func, typename... Args>
    HUH_FORCE_INLINE decltype(auto) ApplyAfter(Func&& func, Args... args) const volatile&& {
        return std::invoke(func, std::forward<Args>(args)..., static_cast<const volatile Pair&&>(*this).first,
                           static_cast<const volatile Pair&&>(*this).second);
    }

    /// Apply's the pair's elements to the beginning of a function call
    /// @param func The function to call
    /// @param args The arguments of the function after pair elements
    /// @return The return value of the function
    template<typename Func, typename... Args>
    HUH_FORCE_INLINE decltype(auto) ApplyBefore(Func&& func, Args... args) & {
        return std::invoke(func, static_cast<Pair&>(*this).first, static_cast<Pair&>(*this).second,
                           std::forward<Args>(args)...);
    }
    template<typename Func, typename... Args>
    HUH_FORCE_INLINE decltype(auto) ApplyBefore(Func&& func, Args... args) const& {
        return std::invoke(func, static_cast<const Pair&>(*this).first, static_cast<const Pair&>(*this).second,
                           std::forward<Args>(args)...);
    }
    template<typename Func, typename... Args>
    HUH_FORCE_INLINE decltype(auto) ApplyBefore(Func&& func, Args... args) volatile& {
        return std::invoke(func, static_cast<volatile Pair&>(*this).first, static_cast<volatile Pair&>(*this).second,
                           std::forward<Args>(args)...);
    }
    template<typename Func, typename... Args>
    HUH_FORCE_INLINE decltype(auto) ApplyBefore(Func&& func, Args... args) const volatile& {
        return std::invoke(func, static_cast<const volatile Pair&>(*this).first,
                           static_cast<const volatile Pair&>(*this).second, std::forward<Args>(args)...);
    }
    template<typename Func, typename... Args>
    HUH_FORCE_INLINE decltype(auto) ApplyBefore(Func&& func, Args... args) && {
        return std::invoke(func, static_cast<Pair&&>(*this).first, static_cast<Pair&&>(*this).second,
                           std::forward<Args>(args)...);
    }
    template<typename Func, typename... Args>
    HUH_FORCE_INLINE decltype(auto) ApplyBefore(Func&& func, Args... args) const&& {
        return std::invoke(func, static_cast<const Pair&&>(*this).first, static_cast<const Pair&&>(*this).second,
                           std::forward<Args>(args)...);
    }
    template<typename Func, typename... Args>
    HUH_FORCE_INLINE decltype(auto) ApplyBefore(Func&& func, Args... args) volatile&& {
        return std::invoke(func, static_cast<volatile Pair&&>(*this).first, static_cast<volatile Pair&&>(*this).second,
                           std::forward<Args>(args)...);
    }

    template<typename Func, typename... Args>
    HUH_FORCE_INLINE decltype(auto) ApplyBefore(Func&& func, Args... args) const volatile&& {
        return std::invoke(func, static_cast<const volatile Pair&&>(*this).first,
                           static_cast<const volatile Pair&&>(*this).second, std::forward<Args>(args)...);
    }
};

template<Uint64 N, PairType Type>
    requires(N < 2)
decltype(auto) get(Type&& tuple) {
    return std::forward<Type>(tuple).template Get<N>();
}

}// namespace HUH

template<typename T1, typename T2>
// NOLINTNEXTLINE(*-dcl58-cpp)
struct std::tuple_size<HUH::Pair<T1, T2>> : std::integral_constant<std::size_t, 2> {};

template<std::size_t N, typename T1, typename T2>
// NOLINTNEXTLINE(*-dcl58-cpp)
struct std::tuple_element<N, HUH::Pair<T1, T2>> {
    using type = HUH::Pair<T1, T2>::template ElementType<N>;
};
