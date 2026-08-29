#pragma once

#include <HUH/definitions.h>

#include <cstddef>

// ClassFuncTypeHelpe

namespace HUH {
template<bool IsConst, typename ClassType, typename Function>
struct ClassFuncTypeHelper;

template<typename ClassType, typename RetType, typename... Args>
struct ClassFuncTypeHelper<true, ClassType, RetType(Args...)> {
    typedef RetType (ClassType::*Type)(Args...) const;
};

template<typename ClassType, typename RetType, typename... Args>
struct ClassFuncTypeHelper<false, ClassType, RetType(Args...)> {
    typedef RetType (ClassType::*Type)(Args...);
};

template<bool IsConst, typename ClassType, typename Function>
using ClassFuncTypeHelper_t = ClassFuncTypeHelper<IsConst, ClassType, Function>::Type;

template<typename T, typename R, R T::* M>
HUH_FORCE_INLINE size_t OffsetOf() {
    return reinterpret_cast<size_t>(&(((T*)0)->*M));
};

template<auto M>
struct ClassMemberTypeHelper;

template<class ClassType, class MemberType, MemberType ClassType::* M>
struct ClassMemberTypeHelper<M> {
    typedef MemberType Type;
    typedef ClassType StructType;
    inline static size_t Offset = OffsetOf<ClassType, MemberType, M>();
};

// Copy Cv

template<typename From, typename To>
struct CopyCV {
    typedef To Type;
};
template<typename From, typename To>
struct CopyCV<const From, To> {
    typedef const To Type;
};
template<typename From, typename To>
struct CopyCV<volatile From, To> {
    typedef volatile To Type;
};

template<typename From, typename To>
struct CopyCV<const volatile From, To> {
    typedef const volatile To Type;
};

template<typename From, typename To>
using CopyCV_t = CopyCV<From, To>::Type;

// Copy Ref

template<typename From, typename To>
struct CopyRef {
    typedef To Type;
};
template<typename From, typename To>
struct CopyRef<From, To&> {
    typedef To& Type;
};
template<typename From, typename To>
struct CopyRef<From, To&&> {
    typedef To&& Type;
};
template<typename From, typename To>
struct CopyRef<From&, To> {
    typedef To& Type;
};
template<typename From, typename To>
struct CopyRef<From&&, To> {
    typedef To&& Type;
};
template<typename From, typename To>
struct CopyRef<From&, To&> {
    typedef To& Type;
};
template<typename From, typename To>
struct CopyRef<From&, To&&> {
    typedef To& Type;
};
template<typename From, typename To>
struct CopyRef<From&&, To&> {
    typedef To& Type;
};
template<typename From, typename To>
struct CopyRef<From&&, To&&> {
    typedef To&& Type;
};

template<typename From, typename To>
using CopyRef_t = CopyRef<From, To>::Type;

// CopyCVRef

template<typename From, typename To>
struct CopyCVRef {
    typedef CopyCV_t<From, To> Type;
};
template<typename From, typename To>
struct CopyCVRef<From, To&> {
    typedef CopyCV_t<From, To>& Type;
};
template<typename From, typename To>
struct CopyCVRef<From, To&&> {
    typedef CopyCV_t<From, To>&& Type;
};
template<typename From, typename To>
struct CopyCVRef<From&, To> {
    typedef CopyCV_t<From, To>& Type;
};
template<typename From, typename To>
struct CopyCVRef<From&&, To> {
    typedef CopyCV_t<From, To>&& Type;
};
template<typename From, typename To>
struct CopyCVRef<From&, To&> {
    typedef CopyCV_t<From, To>& Type;
};
template<typename From, typename To>
struct CopyCVRef<From&, To&&> {
    typedef CopyCV_t<From, To>& Type;
};
template<typename From, typename To>
struct CopyCVRef<From&&, To&> {
    typedef CopyCV_t<From, To>& Type;
};
template<typename From, typename To>
struct CopyCVRef<From&&, To&&> {
    typedef CopyCV_t<From, To>&& Type;
};

template<typename From, typename To>
using CopyCVRef_t = CopyCVRef<From, To>::Type;

}// namespace HUH
