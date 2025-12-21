#pragma once
#include <string>
#include <format>
#include <tuple>
#include <vector>
#include <HUH/template_helpers.h>
#include <oneapi/tbb/profiling.h>

namespace HUH {

template<typename Func>
class ICommonEventBase;

template<typename Func>
class Event;

template<typename Func>
class MultiEvent;

template<typename Func, typename FunctionType, typename... CommonArgs>
class FunctorEvent;

template<bool IsConst, typename Func, typename ClassType, typename... CommonArgs>
class ClassEvent;

template<typename Func, typename... CommonArgs>
class RawEvent;

class EventHandler {
public:
    template<typename Func>
    friend class Event;

    template<typename Tp, typename CharT>
    friend struct std::formatter;

    EventHandler() = default;

    bool operator==(const EventHandler& rhs) const { return this->m_id == rhs.m_id; }
    bool operator!=(const EventHandler& rhs) const { return !(*this == rhs); }

private:
    EventHandler(Uint64 new_id) : m_id(new_id) {}
    static Uint64 GetNextEventId();

    Uint64 m_id = 0;
};

template<typename RetType, typename... Args>
class ICommonEventBase<RetType(Args...)> {
public:
    virtual ~ICommonEventBase() = default;
    virtual RetType Execute(Args...) = 0;
};

template<typename Func, typename... CommonArgs>
class IEventBase : public ICommonEventBase<Func> {
public:
    explicit IEventBase(CommonArgs&&... args) : m_commonArgs(std::forward<CommonArgs>(args)...) {}
    virtual ~IEventBase() = default;

    // template<typename FunctionType, typename... Args, size_t... Is>
    // auto ApplyAfterIndex(FunctionType&& function, Args&&... args, std::index_sequence<Is...>) const {
    //     return std::invoke(std::forward<FunctionType>(function), std::forward<Args>(args)...,
    //                        std::get<Is>(m_commonArgs)...);
    // }

    template<typename FunctionType, typename... Args>
    auto ApplyAfter(FunctionType&& function, Args&&... args) {
        // TODO THIS IS HORRIBLE CHANGE IT LATER WORKS FOR NOW PROBABLY NEED MY OWN TUPLE
        // return std::invoke(std::forward<FunctionType>(function), std::forward<Args>(args)...);
        return std::apply(std::forward<FunctionType>(function),
                          std::tuple_cat(std::tuple<Args...>(args...), m_commonArgs));
        // auto sequence = ;
        // return ApplyAfterIndex(std::forward<FunctionType>(function), std::forward<Args>(args)...,
        //                        std::make_index_sequence<size>{});
    }

protected:
    std::tuple<CommonArgs...> m_commonArgs;
    static constexpr size_t size = std::tuple_size_v<std::tuple<CommonArgs...>>;
};

template<typename RetType, typename... Args, typename FunctionType, typename... CommonArgs>
class FunctorEvent<RetType(Args...), FunctionType, CommonArgs...> : public IEventBase<RetType(Args...), CommonArgs...> {
public:
    template<typename InFunctionType, typename... InCommonArgs>
    explicit FunctorEvent(InFunctionType functor, InCommonArgs&&... commonArgs)
        : IEventBase<RetType(Args...), CommonArgs...>(std::forward<InCommonArgs>(commonArgs)...),
          m_functor(functor) {}

    RetType Execute(Args... args) override {
        return this->template ApplyAfter<decltype(m_functor), Args...>(std::forward<decltype(m_functor)>(m_functor),
                                                                       std::forward<Args>(args)...);
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
    explicit ClassEvent(ClassType* inClass, FuncType inFunction, InCommonArgs&&... commonArgs)
        : IEventBase<RetType(Args...), CommonArgs...>(std::forward<InCommonArgs>(commonArgs)...),
          m_class(inClass),
          m_function(inFunction) {}

    RetType Execute(Args... args) override {
        using NoConstClass = std::remove_const_t<ClassType>;
        auto* classPtr = const_cast<NoConstClass*>(m_class);
        return this->template ApplyAfter<FuncType, ClassType*, Args...>(
            std::forward<FuncType>(m_function), std::forward<ClassType*>(classPtr), std::forward<Args>(args)...);
    }

private:
    ClassType* m_class;
    FuncType m_function;
};

template<typename RetType, typename... Args, typename... CommonArgs>
class RawEvent<RetType(Args...), CommonArgs...> : public IEventBase<RetType(Args...), CommonArgs...> {
public:
    using RawFunctionType = RetType(Args..., CommonArgs...);

    template<typename... InCommonArgs>
    explicit RawEvent(RawFunctionType* InFunction, InCommonArgs&&... commonArgs)
        : IEventBase<RetType(Args...), CommonArgs...>(std::forward<InCommonArgs>(commonArgs)...),
          m_rawFunction(InFunction) {}

    RetType Execute(Args... args) override {
        return this->template ApplyAfter<RawFunctionType*, Args...>(std::forward<RawFunctionType*>(m_rawFunction),
                                                                    std::forward<Args>(args)...);
    }

private:
    RawFunctionType* m_rawFunction = nullptr;
};

template<typename RetType, typename... Args>
class Event<RetType(Args...)> {
public:
    Event() : m_event(nullptr), m_handler(EventHandler::GetNextEventId()) {}
    ~Event() { delete m_event; }

    template<typename FunctionType, typename... CommonArgs>
    void BindLambda(FunctionType InFunction, CommonArgs&&... InCommonArgs) {
        delete m_event;
        m_event =
            new FunctorEvent<RetType(Args...), std::remove_reference_t<FunctionType>, std::decay_t<CommonArgs>...>(
                InFunction, std::forward<CommonArgs>(InCommonArgs)...);
    }

    template<typename ClassType, typename... CommonArgs>
    void BindClass(
        ClassType* inClass,
        ClassFuncTypeHelper<false, ClassType, RetType(Args..., std::decay_t<CommonArgs>...)>::Type inFunction,
        CommonArgs&&... inCommonArgs) {
        delete m_event;
        m_event = new ClassEvent<false, RetType(Args...), ClassType, std::decay_t<CommonArgs>...>(
            inClass, inFunction, std::forward<CommonArgs>(inCommonArgs)...);
    }

    template<typename ClassType, typename... CommonArgs>
    void BindClass(ClassType* inClass,
                   ClassFuncTypeHelper<true, ClassType, RetType(Args..., std::decay_t<CommonArgs>...)>::Type inFunction,
                   CommonArgs&&... inCommonArgs) {
        delete m_event;
        m_event = new ClassEvent<true, RetType(Args...), ClassType, std::decay_t<CommonArgs>...>(
            inClass, inFunction, std::forward<CommonArgs>(inCommonArgs)...);
    }

    template<typename... CommonArgs>
    void BindRaw(RetType (*InFunction)(Args..., CommonArgs...), CommonArgs&&... commonArgs) {
        delete m_event;
        m_event = new RawEvent<RetType(Args...), std::decay_t<CommonArgs>...>(InFunction,
                                                                              std::forward<CommonArgs>(commonArgs)...);
    }

    RetType Execute(Args... args) const { return m_event->Execute(args...); }

    HUH_NODISCARD EventHandler GetHandler() const { return m_handler; }

private:
    ICommonEventBase<RetType(Args...)>* m_event = nullptr;
    EventHandler m_handler;
};

template<typename RetType, typename... Args>
class MultiEvent<RetType(Args...)> {
public:
    ~MultiEvent() {
        for (auto event : m_eventList) {
            delete event;
        }
    }

    template<typename FunctionType, typename... CommonArgs>
    EventHandler AddLambda(FunctionType&& InFunction, CommonArgs&&... InCommonArgs) {
        m_eventList.push_back(new Event<RetType(Args...)>());
        m_eventList.back()->BindLambda(std::forward<FunctionType>(InFunction),
                                       std::forward<CommonArgs>(InCommonArgs)...);
        return m_eventList.back()->GetHandler();
    }

    template<typename ClassType, typename... CommonArgs>
    EventHandler AddClass(
        ClassType* inClass,
        ClassFuncTypeHelper<false, ClassType, RetType(Args..., std::decay_t<CommonArgs>...)>::Type inFunction,
        CommonArgs&&... inCommonArgs) {
        m_eventList.push_back(new Event<RetType(Args...)>());
        m_eventList.back()->BindClass(inClass, inFunction, std::forward<CommonArgs>(inCommonArgs)...);
        return m_eventList.back()->GetHandler();
    }

    template<typename ClassType, typename... CommonArgs>
    EventHandler AddClass(
        ClassType* inClass,
        ClassFuncTypeHelper<true, ClassType, RetType(Args..., std::decay_t<CommonArgs>...)>::Type inFunction,
        CommonArgs&&... inCommonArgs) {
        m_eventList.push_back(new Event<RetType(Args...)>());
        m_eventList.back()->BindClass(inClass, inFunction, std::forward<CommonArgs>(inCommonArgs)...);
        return m_eventList.back()->GetHandler();
    }

    template<typename... CommonArgs>
    EventHandler AddRaw(RetType (*InFunction)(Args..., CommonArgs...), CommonArgs&&... commonArgs) {
        m_eventList.push_back(new Event<RetType(Args...)>());
        m_eventList.back()->BindRaw(InFunction, std::forward<CommonArgs>(commonArgs)...);
        return m_eventList.back()->GetHandler();
    }

    void Remove(const EventHandler& handler) {
        for (size_t i = 0; i < m_eventList.size(); ++i) {
            if (m_eventList[i]->GetHandler() == handler) {
                delete m_eventList[i];
                m_eventList.erase(m_eventList.begin() + i);
            }
        }
    }

    void ExecuteAll(Args... args) const {
        if (m_eventList.empty()) {
            return;
        }
        for (size_t i = m_eventList.size() - 1; i > 0; --i) {
            m_eventList[i]->Execute(args...);
        }
        m_eventList[0]->Execute(args...);
    }

private:
    std::vector<Event<RetType(Args...)>*> m_eventList;
};

}// namespace HUH

template<>
struct std::formatter<HUH::EventHandler> : std::formatter<std::string> {
    auto format(HUH::EventHandler p, format_context& ctx) const {
        return formatter<string>::format(std::format("{}", p.m_id), ctx);
    }
};
