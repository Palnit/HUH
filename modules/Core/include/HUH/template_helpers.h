#pragma once

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
