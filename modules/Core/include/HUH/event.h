#pragma once
#include <tuple>
#include <HUH/template_helpers.h>

namespace HUH {

template<typename Func>
class ICommonEventBase;

template<typename RetType, typename... Args>
class ICommonEventBase<RetType(Args...)> {
public:
    virtual ~ICommonEventBase() = default;
    virtual RetType Execute(Args...) const = 0;
};

template<typename Func, typename... CommonArgs>
class IEventBase : public ICommonEventBase<Func> {
public:
    explicit IEventBase(CommonArgs&&... args) : m_commonArgs(std::forward<CommonArgs>(args)...) {}
    virtual ~IEventBase() = default;

    // template<typename... Args, typename FunctionType, size_t... Is>
    // auto ApplyAfter(Args&&... args, FunctionType&& function, std::index_sequence<Is...>) const {
    //     return std::invoke(std::forward<FunctionType>(function), std::forward<Args>(args)...,
    //                        std::get<Is>(m_commonArgs)...);
    // }

    template<typename FunctionType, typename... Args>
    auto ApplyAfter(FunctionType&& function, Args&&... args) const {
        // TODO THIS IS HORRIBLE CHANGE IT LATER WORKS FOR NOW
        std::apply(std::forward<FunctionType>(function),
                   std::tuple_cat(std::tuple(std::forward<Args>(args)...), m_commonArgs));
        // auto sequence = std::make_index_sequence<size>{};
        // return ApplyAfter<Args..., FunctionType>(std::forward<Args>(args)..., std::forward<FunctionType>(function),
        //                                          sequence);
    }

protected:
    std::tuple<CommonArgs...> m_commonArgs;
    static constexpr size_t size = std::tuple_size_v<std::tuple<CommonArgs...>>;
};

template<typename Func, typename FunctionType, typename... CommonArgs>
class FunctorEvent;

template<bool IsConst, typename Func, typename ClassType, typename... CommonArgs>
class ClassEvent;

template<typename RetType, typename... Args, typename FunctionType, typename... CommonArgs>
class FunctorEvent<RetType(Args...), FunctionType, CommonArgs...> : public IEventBase<RetType(Args...), CommonArgs...> {
public:
    template<typename InFunctionType, typename... InCommonArgs>
    explicit FunctorEvent(InFunctionType&& functor, InCommonArgs&&... commonArgs)
        : IEventBase<RetType(Args...), CommonArgs...>(std::forward<InCommonArgs>(commonArgs)...),
          m_functor(std::forward<InFunctionType>(functor)) {}

    RetType Execute(Args... args) const override {
        return this->ApplyAfter(std::forward<decltype(m_functor)>(m_functor), std::forward<Args>(args)...);
    }

private:
    mutable std::remove_const_t<FunctionType> m_functor;
};

template<bool IsConst, typename RetType, typename... Args, typename ClassType, typename... CommonArgs>
class ClassEvent<IsConst, RetType(Args...), ClassType, CommonArgs...>
    : public IEventBase<RetType(Args...), CommonArgs...> {
public:
    using FuncType = ClassFuncTypeHelper<IsConst, ClassType, RetType(Args..., std::decay_t<CommonArgs>...)>::Type;

    template<typename... InCommonArgs>
    ClassEvent(ClassType* inClass, FuncType inFunction, CommonArgs&&... commonArgs)
        : IEventBase<RetType(Args...), CommonArgs...>(std::forward<CommonArgs>(commonArgs)...),
          m_class(inClass),
          m_function(inFunction) {}

    RetType Execute(Args... args) const override {
        using NoConstClass = std::remove_const_t<ClassType>;
        NoConstClass* classPtr = const_cast<NoConstClass*>(m_class);
        return this->ApplyAfter(m_function, classPtr, std::forward<Args>(args)...);
    }

private:
    ClassType* m_class;
    FuncType m_function;
};

template<typename RetType, typename... Args>
class Event {
public:
    ~Event() { delete m_event; };

    template<typename FunctionType, typename... CommonArgs>
    void BindLambda(FunctionType&& InFunction, CommonArgs&&... InCommonArgs) {
        delete m_event;
        m_event =
            new FunctorEvent<RetType(Args...), std::remove_reference_t<FunctionType>, std::decay_t<CommonArgs>...>(
                std::forward<FunctionType>(InFunction), std::forward<CommonArgs>(InCommonArgs)...);
    }

    template<typename ClassType, typename... CommonArgs>
    void BindClass(
        ClassType* inClass,
        typename ClassFuncTypeHelper<false, ClassType, RetType(Args..., std::decay_t<CommonArgs>...)>::Type inFunction,
        CommonArgs&&... inCommonArgs) {
        delete m_event;
        m_event = new ClassEvent<false, RetType(Args...), ClassType, std::decay_t<CommonArgs>...>(
            inClass, inFunction, std::forward<CommonArgs>(inCommonArgs)...);
    }

    template<typename ClassType, typename... CommonArgs>
    void BindClass(
        ClassType* inClass,
        typename ClassFuncTypeHelper<true, ClassType, RetType(Args..., std::decay_t<CommonArgs>...)>::Type inFunction,
        CommonArgs&&... inCommonArgs) {
        delete m_event;
        m_event = new ClassEvent<false, RetType(Args...), ClassType, std::decay_t<CommonArgs>...>(
            inClass, inFunction, std::forward<CommonArgs>(inCommonArgs)...);
    }

    RetType Execute(Args... args) const { return m_event->Execute(std::forward<Args>(args)...); }

private:
    ICommonEventBase<RetType(Args...)>* m_event = nullptr;
};

}// namespace HUH