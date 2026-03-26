#pragma once

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

template<typename MemberPtr>
struct ClassMemberTypeHelper;

template<class ClassType, class MemberType>
struct ClassMemberTypeHelper<MemberType ClassType::*> {
    typedef MemberType MType;
    typedef ClassType CType;
};

template<class ClassType, class MemberType, MemberType ClassType::* M>
struct ClassMemberHelper {};

template<typename MemberPtr>
struct ClassMemberHelper<ClassMemberTypeHelper<MemberPtr>::CType, ClassMemberTypeHelper<MemberPtr>::MType, MemberPtr> {
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
