#pragma once
#include "HUH/concepts.h"
#include "HUH/definitions.h"
#include "HUH/templates.h"
#include <HUH/logging.h>

#include <HUH/types.h>

namespace HUH {

template<typename... Types>
struct Tuple;

template<typename T, typename... Types>
constexpr Uint64 TupleTypesCount = 0;

template<typename T, typename U, typename... Types>
constexpr Uint64 TupleTypesCount<T, U, Types...> = TupleTypesCount<T, Types...> + (HUH::Same<T, U> ? 1 : 0);

namespace Private {

template<typename T, Uint64 Index, Uint64 Size>
struct TupleElement {
    template<typename U>
        requires(Constructable<U, T>)
    explicit TupleElement(U&& element) : m_element(std::forward<U>(element)) {}

    TupleElement()
        requires(HUH::DefaultConstructable<T>)
        : m_element() {}

    TupleElement(TupleElement&&) = default;
    TupleElement& operator=(TupleElement&&) = default;

    TupleElement(const TupleElement&) = default;
    TupleElement& operator=(const TupleElement&) = default;

    T m_element;
};

template<typename T, typename B>
    requires(Convertable<std::remove_reference_t<T>*, const volatile B*>)
HUH_CONSTEXPR_FORCE decltype(auto) ForwardBase(std::remove_reference_t<T>& obj) {
    return static_cast<HUH::CopyCVRef_t<T&&, B>>(obj);
}

template<Uint64 Index, Uint64 Size>
struct TupleElementGetterByIndex {

    template<typename Type, typename TupleType>
    HUH_NODISCARD HUH_CONSTEXPR_FORCE static decltype(auto) GetImpl(const volatile TupleElement<Type, Index, Size>&,
                                                                    TupleType&& tuple) {
        return (ForwardBase<TupleType, TupleElement<Type, Index, Size>>(tuple).m_element);
    }

    template<typename TupleType>
    HUH_NODISCARD HUH_CONSTEXPR_FORCE static decltype(auto) Get(TupleType&& tuple) {
        return GetImpl(tuple, std::forward<TupleType>(tuple));
    }
};

template<typename Type, Uint64 Size>
struct TupleElementGetterByType {

    template<Uint64 Index, typename TupleType>
    HUH_NODISCARD HUH_CONSTEXPR_FORCE static decltype(auto) GetImpl(const volatile TupleElement<Type, Index, Size>&,
                                                                    TupleType&& tuple) {
        return (ForwardBase<TupleType, TupleElement<Type, Index, Size>>(tuple).m_element);
    }

    template<typename TupleType>
    HUH_NODISCARD HUH_CONSTEXPR_FORCE static decltype(auto) Get(TupleType&& tuple) {
        return GetImpl(tuple, std::forward<TupleType>(tuple));
    }
};

template<typename Indices, typename... Types>
struct TupleBase;

template<typename T>
struct is_tuple_base : std::false_type {};

template<typename Indices, typename... Types>
struct is_tuple_base<TupleBase<Indices, Types...>> : std::true_type {};

template<typename T>
concept TupleBaseType = is_tuple_base<std::remove_cvref_t<T>>::value;

template<typename T>
struct is_tuple : std::false_type {};

template<typename... Types>
struct is_tuple<Tuple<Types...>> : std::true_type {};

template<typename T>
concept TupleType = is_tuple<std::remove_cvref_t<T>>::value;

template<Uint64... Indices, typename... Types>
struct TupleBase<std::integer_sequence<Uint64, Indices...>, Types...>
    : TupleElement<Types, Indices, sizeof...(Types)>... {

    TupleBase() = default;
    TupleBase(TupleBase&&) = default;
    TupleBase& operator=(TupleBase&&) = default;
    TupleBase(const TupleBase&) = default;
    TupleBase& operator=(const TupleBase&) = default;

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

public:
    template<Uint64 Index>
        requires(Index < sizeof...(Types))
    using ElementType = ElementTypeHelper<Index, 0, Types...>::Type;

    template<typename... Args>
        requires(Constructable<Args, TupleElement<Types, Indices, sizeof...(Types)>> && ...)
    explicit TupleBase(Args&&... args) : TupleElement<Types, Indices, sizeof...(Types)>(std::forward<Args>(args))... {}

    template<typename OtherTuple>
        requires(TupleBaseType<std::remove_cvref_t<OtherTuple>> || TupleType<std::remove_cvref_t<OtherTuple>>)
    explicit TupleBase(OtherTuple&& other)
        : TupleElement<Types, Indices, sizeof...(Types)>(std::forward<OtherTuple>(other).template Get<Indices>())... {}

    /// Getter by Index
    /// @tparam Index The Index of the element we want
    template<Uint64 Index>
        requires(Index < sizeof...(Types))
    HUH_FORCE_INLINE decltype(auto) Get() & {
        return TupleElementGetterByIndex<Index, sizeof...(Types)>::Get(static_cast<TupleBase&>(*this));
    }

    template<Uint64 Index>
        requires(Index < sizeof...(Types))
    HUH_FORCE_INLINE decltype(auto) Get() const& {
        return TupleElementGetterByIndex<Index, sizeof...(Types)>::Get(static_cast<const TupleBase&>(*this));
    }

    template<Uint64 Index>
        requires(Index < sizeof...(Types))
    HUH_FORCE_INLINE decltype(auto) Get() volatile& {
        return TupleElementGetterByIndex<Index, sizeof...(Types)>::Get(static_cast<volatile TupleBase&>(*this));
    }

    template<Uint64 Index>
        requires(Index < sizeof...(Types))
    HUH_FORCE_INLINE decltype(auto) Get() const volatile& {
        return TupleElementGetterByIndex<Index, sizeof...(Types)>::Get(static_cast<const volatile TupleBase&>(*this));
    }

    template<Uint64 Index>
        requires(Index < sizeof...(Types))
    HUH_FORCE_INLINE decltype(auto) Get() && {
        return TupleElementGetterByIndex<Index, sizeof...(Types)>::Get(static_cast<TupleBase&&>(*this));
    }

    template<Uint64 Index>
        requires(Index < sizeof...(Types))
    HUH_FORCE_INLINE decltype(auto) Get() const&& {
        return TupleElementGetterByIndex<Index, sizeof...(Types)>::Get(static_cast<const TupleBase&&>(*this));
    }

    template<Uint64 Index>
        requires(Index < sizeof...(Types))
    HUH_FORCE_INLINE decltype(auto) Get() volatile&& {
        return TupleElementGetterByIndex<Index, sizeof...(Types)>::Get(static_cast<volatile TupleBase&&>(*this));
    }

    template<Uint64 Index>
        requires(Index < sizeof...(Types))
    HUH_FORCE_INLINE decltype(auto) Get() const volatile&& {
        return TupleElementGetterByIndex<Index, sizeof...(Types)>::Get(static_cast<const volatile TupleBase&&>(*this));
    }

    /// Getter by type if only one type exits in the tuple
    /// @tparam T The type we are requesting from the tuple
    template<typename T>
        requires(TupleTypesCount<T, Types...> == 1)
    HUH_FORCE_INLINE decltype(auto) Get() & {
        return TupleElementGetterByType<T, sizeof...(Types)>::Get(static_cast<TupleBase&>(*this));
    }

    template<typename T>
        requires(TupleTypesCount<T, Types...> == 1)
    HUH_FORCE_INLINE decltype(auto) Get() const& {
        return TupleElementGetterByType<T, sizeof...(Types)>::Get(static_cast<const TupleBase&>(*this));
    }

    template<typename T>
        requires(TupleTypesCount<T, Types...> == 1)
    HUH_FORCE_INLINE decltype(auto) Get() volatile& {
        return TupleElementGetterByType<T, sizeof...(Types)>::Get(static_cast<volatile TupleBase&>(*this));
    }

    template<typename T>
        requires(TupleTypesCount<T, Types...> == 1)
    HUH_FORCE_INLINE decltype(auto) Get() const volatile& {
        return TupleElementGetterByType<T, sizeof...(Types)>::Get(static_cast<const volatile TupleBase&>(*this));
    }

    template<typename T>
        requires(TupleTypesCount<T, Types...> == 1)
    HUH_FORCE_INLINE decltype(auto) Get() && {
        return TupleElementGetterByType<T, sizeof...(Types)>::Get(static_cast<TupleBase&&>(*this));
    }

    template<typename T>
        requires(TupleTypesCount<T, Types...> == 1)
    HUH_FORCE_INLINE decltype(auto) Get() const&& {
        return TupleElementGetterByType<T, sizeof...(Types)>::Get(static_cast<const TupleBase&&>(*this));
    }

    template<typename T>
        requires(TupleTypesCount<T, Types...> == 1)
    HUH_FORCE_INLINE decltype(auto) Get() volatile&& {
        return TupleElementGetterByType<T, sizeof...(Types)>::Get(static_cast<volatile TupleBase&&>(*this));
    }

    template<typename T>
        requires(TupleTypesCount<T, Types...> == 1)
    HUH_FORCE_INLINE decltype(auto) Get() const volatile&& {
        return TupleElementGetterByType<T, sizeof...(Types)>::Get(static_cast<const volatile TupleBase&&>(*this));
    }

    /// Apply's the tuple's elements to the end of a function call
    /// @param func The function to call
    /// @param args The arguments of the function before tuple elements
    /// @return The return value of the function
    template<typename Func, typename... Args>
    HUH_FORCE_INLINE decltype(auto) ApplyAfter(Func&& func, Args... args) & {
        return std::invoke(func, std::forward<Args>(args)..., static_cast<TupleBase&>(*this).Get<Indices>()...);
    }
    template<typename Func, typename... Args>
    HUH_FORCE_INLINE decltype(auto) ApplyAfter(Func&& func, Args... args) const& {
        return std::invoke(func, std::forward<Args>(args)..., static_cast<const TupleBase&>(*this).Get<Indices>()...);
    }
    template<typename Func, typename... Args>
    HUH_FORCE_INLINE decltype(auto) ApplyAfter(Func&& func, Args... args) volatile& {
        return std::invoke(func, std::forward<Args>(args)...,
                           static_cast<volatile TupleBase&>(*this).Get<Indices>()...);
    }
    template<typename Func, typename... Args>
    HUH_FORCE_INLINE decltype(auto) ApplyAfter(Func&& func, Args... args) const volatile& {
        return std::invoke(func, std::forward<Args>(args)...,
                           static_cast<const volatile TupleBase&>(*this).Get<Indices>()...);
    }
    template<typename Func, typename... Args>
    HUH_FORCE_INLINE decltype(auto) ApplyAfter(Func&& func, Args... args) && {
        return std::invoke(func, std::forward<Args>(args)..., static_cast<TupleBase&&>(*this).Get<Indices>()...);
    }
    template<typename Func, typename... Args>
    HUH_FORCE_INLINE decltype(auto) ApplyAfter(Func&& func, Args... args) const&& {
        return std::invoke(func, std::forward<Args>(args)..., static_cast<const TupleBase&&>(*this).Get<Indices>()...);
    }
    template<typename Func, typename... Args>
    HUH_FORCE_INLINE decltype(auto) ApplyAfter(Func&& func, Args... args) volatile&& {
        return std::invoke(func, std::forward<Args>(args)...,
                           static_cast<volatile TupleBase&&>(*this).Get<Indices>()...);
    }
    template<typename Func, typename... Args>
    HUH_FORCE_INLINE decltype(auto) ApplyAfter(Func&& func, Args... args) const volatile&& {
        return std::invoke(func, std::forward<Args>(args)...,
                           static_cast<const volatile TupleBase&&>(*this).Get<Indices>()...);
    }

    /// Apply's the tuple's elements to the beginning of a function call
    /// @param func The function to call
    /// @param args The arguments of the function after tuple elements
    /// @return The return value of the function
    template<typename Func, typename... Args>
    HUH_FORCE_INLINE decltype(auto) ApplyBefore(Func&& func, Args... args) & {
        return std::invoke(func, static_cast<TupleBase&>(*this).Get<Indices>()..., std::forward<Args>(args)...);
    }
    template<typename Func, typename... Args>
    HUH_FORCE_INLINE decltype(auto) ApplyBefore(Func&& func, Args... args) const& {
        return std::invoke(func, static_cast<const TupleBase&>(*this).Get<Indices>()..., std::forward<Args>(args)...);
    }
    template<typename Func, typename... Args>
    HUH_FORCE_INLINE decltype(auto) ApplyBefore(Func&& func, Args... args) volatile& {
        return std::invoke(func, static_cast<volatile TupleBase&>(*this).Get<Indices>()...,
                           std::forward<Args>(args)...);
    }
    template<typename Func, typename... Args>
    HUH_FORCE_INLINE decltype(auto) ApplyBefore(Func&& func, Args... args) const volatile& {
        return std::invoke(func, static_cast<const volatile TupleBase&>(*this).Get<Indices>()...,
                           std::forward<Args>(args)...);
    }
    template<typename Func, typename... Args>
    HUH_FORCE_INLINE decltype(auto) ApplyBefore(Func&& func, Args... args) && {
        return std::invoke(func, static_cast<TupleBase&&>(*this).Get<Indices>()..., std::forward<Args>(args)...);
    }
    template<typename Func, typename... Args>
    HUH_FORCE_INLINE decltype(auto) ApplyBefore(Func&& func, Args... args) const&& {
        return std::invoke(func, static_cast<const TupleBase&&>(*this).Get<Indices>()..., std::forward<Args>(args)...);
    }
    template<typename Func, typename... Args>
    HUH_FORCE_INLINE decltype(auto) ApplyBefore(Func&& func, Args... args) volatile&& {
        return std::invoke(func, static_cast<volatile TupleBase&&>(*this).Get<Indices>()...,
                           std::forward<Args>(args)...);
    }

    template<typename Func, typename... Args>
    HUH_FORCE_INLINE decltype(auto) ApplyBefore(Func&& func, Args... args) const volatile&& {
        return std::invoke(func, static_cast<const volatile TupleBase&&>(*this).Get<Indices>()...,
                           std::forward<Args>(args)...);
    }

    template<HUH::ComparableWith<Types>... Args>
    HUH_FORCE_INLINE bool operator==(const Tuple<Args...>& other) const {
        return (this->Get<Indices>().operator==(other.template Get<Indices>()) && ...);
    }

    template<HUH::ComparableWith<Types>... Args>
    HUH_FORCE_INLINE bool operator!=(const Tuple<Args...>& other) const {
        return !(*this == other);
    }

    template<HUH::HasLessThan<Types>... Args>
    HUH_FORCE_INLINE bool operator<(const Tuple<Args...>& other) const {
        return (this->Get<Indices>().operator<(other.template Get<Indices>()) && ...);
    }

    template<HUH::HasLessThanEqual<Types>... Args>
    HUH_FORCE_INLINE bool operator<=(const Tuple<Args...>& other) const {
        return (this->Get<Indices>().operator<=(other.template Get<Indices>()) && ...);
    }

    template<HUH::HasGreaterThan<Types>... Args>
    HUH_FORCE_INLINE bool operator>(const Tuple<Args...>& other) const {
        return (this->Get<Indices>().operator>(other.template Get<Indices>()) && ...);
    }

    template<HUH::HasGreaterThanEqual<Types>... Args>
    HUH_FORCE_INLINE bool operator>=(const Tuple<Args...>& other) const {
        return (this->Get<Indices>().operator>=(other.template Get<Indices>()) && ...);
    }
};

template<typename LhsTuple, typename RhsTuple, Uint64... Indices>
HUH_FORCE_INLINE void Assign(LhsTuple& lhs, RhsTuple&& rhs, std::integer_sequence<Uint64, Indices...>) {
    (lhs.template Get<Indices>().operator=(std::forward<RhsTuple>(rhs).template Get<Indices>()), ...);
}

}// namespace Private

template<typename... Types>
struct Tuple : Private::TupleBase<std::make_integer_sequence<Uint64, sizeof...(Types)>, Types...> {
    using Super = Private::TupleBase<std::make_integer_sequence<Uint64, sizeof...(Types)>, Types...>;

    Tuple() = default;
    Tuple(Tuple&&) = default;
    Tuple& operator=(Tuple&&) = default;
    Tuple(const Tuple&) = default;
    Tuple& operator=(const Tuple&) = default;

    template<HUH::Constructable<Types>... Args>
    explicit Tuple(Args&&... args) : Super(std::forward<Args>(args)...) {}

    template<HUH::Constructable<Types>... Args>
    // NOLINTNEXTLINE(google-explicit-constructor)
    Tuple(Tuple<Args...>&& other) : Super(std::move(other)) {}

    template<HUH::Constructable<Types>... Args>
    // NOLINTNEXTLINE(google-explicit-constructor)
    Tuple(const Tuple<Args...>& other) : Super(other) {}

    template<HUH::Assignable<Types>... Args>
    HUH_FORCE_INLINE Tuple& operator=(const Tuple<Args...>& other) {
        HUH::Private::Assign(*this, other, std::make_integer_sequence<Uint64, sizeof...(Types)>{});
        return *this;
    }

    template<HUH::Assignable<Types>... Args>
    HUH_FORCE_INLINE Tuple& operator=(Tuple<Args...>&& other) {
        HUH::Private::Assign(*this, std::move(other), std::make_integer_sequence<Uint64, sizeof...(Types)>{});
        return *this;
    }
};

template<Uint64 N, typename Type>
    requires(HUH::Private::TupleBaseType<std::remove_cvref_t<Type>>
             || HUH::Private::TupleType<std::remove_cvref_t<Type>>)
decltype(auto) get(Type&& tuple) {
    return std::forward<Type>(tuple).template Get<N>();
}

}// namespace HUH

template<typename... Args>
// NOLINTNEXTLINE(*-dcl58-cpp)
struct std::tuple_size<HUH::Tuple<Args...>> : std::integral_constant<std::size_t, sizeof...(Args)> {};

template<std::size_t N, typename... Args>
// NOLINTNEXTLINE(*-dcl58-cpp)
struct std::tuple_element<N, HUH::Tuple<Args...>> {
    using type = HUH::Tuple<Args...>::template ElementType<N>;
};
