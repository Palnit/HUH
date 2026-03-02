#pragma once

#include <string>
#include <format>
#include <vector>
#include <HUH/templates.h>
#include <HUH/concepts.h>
#include <HUH/Types/tuple.h>

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
    // NOLINTNEXTLINE(*-dcl58-cpp)
    friend struct std::formatter;

    HUH_FORCE_INLINE EventHandler() = default;

    HUH_FORCE_INLINE bool operator==(const EventHandler& rhs) const { return this->m_id == rhs.m_id; }
    HUH_FORCE_INLINE bool operator!=(const EventHandler& rhs) const { return !(*this == rhs); }

private:
    HUH_FORCE_INLINE explicit EventHandler(const Uint64 new_id) : m_id(new_id) {}
    HUH_CORE_API static Uint64 GetNextEventId();

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
    HUH_FORCE_INLINE explicit IEventBase(CommonArgs&&... args) : m_commonArgs(std::forward<CommonArgs>(args)...) {}
    HUH_FORCE_INLINE ~IEventBase() override = default;

protected:
    Tuple<CommonArgs...> m_commonArgs;
};

template<typename RetType, typename... Args, typename FunctionType, typename... CommonArgs>
class FunctorEvent<RetType(Args...), FunctionType, CommonArgs...> : public IEventBase<RetType(Args...), CommonArgs...> {
public:
    template<typename InFunctionType, typename... InCommonArgs>
    HUH_FORCE_INLINE explicit FunctorEvent(InFunctionType functor, InCommonArgs&&... commonArgs)
        : IEventBase<RetType(Args...), CommonArgs...>(std::forward<InCommonArgs>(commonArgs)...),
          m_functor(functor) {}

    HUH_FORCE_INLINE RetType Execute(Args... args) override {
        return this->m_commonArgs.template ApplyAfter<decltype(m_functor), Args...>(
            std::forward<decltype(m_functor)>(m_functor), std::forward<Args>(args)...);
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
    HUH_FORCE_INLINE explicit ClassEvent(ClassType* inClass, FuncType inFunction, InCommonArgs&&... commonArgs)
        : IEventBase<RetType(Args...), CommonArgs...>(std::forward<InCommonArgs>(commonArgs)...),
          m_class(inClass),
          m_function(inFunction) {}

    HUH_FORCE_INLINE RetType Execute(Args... args) override {
        using NoConstClass = std::remove_const_t<ClassType>;
        auto* classPtr = const_cast<NoConstClass*>(m_class);
        return this->m_commonArgs.template ApplyAfter<FuncType, ClassType*, Args...>(
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
    HUH_FORCE_INLINE explicit RawEvent(RawFunctionType* InFunction, InCommonArgs&&... commonArgs)
        : IEventBase<RetType(Args...), CommonArgs...>(std::forward<InCommonArgs>(commonArgs)...),
          m_rawFunction(InFunction) {}

    HUH_FORCE_INLINE RetType Execute(Args... args) override {
        return this->m_commonArgs.template ApplyAfter<RawFunctionType*, Args...>(
            std::forward<RawFunctionType*>(m_rawFunction), std::forward<Args>(args)...);
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
    HUH_FORCE_INLINE void Bind(FunctionType InFunction, CommonArgs&&... InCommonArgs) {
        delete m_event;
        m_event =
            new FunctorEvent<RetType(Args...), std::remove_reference_t<FunctionType>, std::decay_t<CommonArgs>...>(
                InFunction, std::forward<CommonArgs>(InCommonArgs)...);
    }

    template<typename ClassType, typename... CommonArgs, typename = std::enable_if_t<std::is_class_v<ClassType>>>
    HUH_FORCE_INLINE void Bind(
        ClassType* inClass,
        ClassFuncTypeHelper<false, ClassType, RetType(Args..., std::decay_t<CommonArgs>...)>::Type inFunction,
        CommonArgs&&... inCommonArgs) {
        delete m_event;
        m_event = new ClassEvent<false, RetType(Args...), ClassType, std::decay_t<CommonArgs>...>(
            inClass, inFunction, std::forward<CommonArgs>(inCommonArgs)...);
    }

    template<typename ClassType, typename... CommonArgs, typename = std::enable_if_t<std::is_class_v<ClassType>>>
    HUH_FORCE_INLINE void Bind(
        ClassType* inClass,
        ClassFuncTypeHelper<true, ClassType, RetType(Args..., std::decay_t<CommonArgs>...)>::Type inFunction,
        CommonArgs&&... inCommonArgs) {
        delete m_event;
        m_event = new ClassEvent<true, RetType(Args...), ClassType, std::decay_t<CommonArgs>...>(
            inClass, inFunction, std::forward<CommonArgs>(inCommonArgs)...);
    }

    template<typename... CommonArgs>
    HUH_FORCE_INLINE void Bind(RetType (*InFunction)(Args..., CommonArgs...), CommonArgs&&... commonArgs) {
        delete m_event;
        m_event = new RawEvent<RetType(Args...), std::decay_t<CommonArgs>...>(InFunction,
                                                                              std::forward<CommonArgs>(commonArgs)...);
    }

    HUH_FORCE_INLINE RetType Execute(Args... args) const { return m_event->Execute(args...); }

    HUH_FORCE_INLINE RetType operator()(Args... args) const { return m_event->Execute(args...); }

    HUH_NODISCARD EventHandler GetHandler() const { return m_handler; }

private:
    ICommonEventBase<RetType(Args...)>* m_event = nullptr;
    EventHandler m_handler;
};

template<typename RetType, typename... Args>
class MultiEvent<RetType(Args...)> {
public:
    HUH_FORCE_INLINE ~MultiEvent() {
        for (auto event : m_eventList) {
            delete event;
        }
    }

    template<typename FunctionType, typename... CommonArgs>
    HUH_FORCE_INLINE EventHandler Add(FunctionType&& InFunction, CommonArgs&&... InCommonArgs) {
        m_eventList.push_back(new Event<RetType(Args...)>());
        m_eventList.back()->Bind(std::forward<FunctionType>(InFunction), std::forward<CommonArgs>(InCommonArgs)...);
        return m_eventList.back()->GetHandler();
    }

    template<typename ClassType, typename... CommonArgs, typename = std::enable_if_t<std::is_class_v<ClassType>>>
    HUH_NODISCARD HUH_FORCE_INLINE EventHandler
    Add(ClassType* inClass,
        ClassFuncTypeHelper<false, ClassType, RetType(Args..., std::decay_t<CommonArgs>...)>::Type inFunction,
        CommonArgs&&... inCommonArgs) {
        m_eventList.push_back(new Event<RetType(Args...)>());
        m_eventList.back()->Bind(inClass, inFunction, std::forward<CommonArgs>(inCommonArgs)...);
        return m_eventList.back()->GetHandler();
    }

    template<typename ClassType, typename... CommonArgs, typename = std::enable_if_t<std::is_class_v<ClassType>>>
    HUH_NODISCARD HUH_FORCE_INLINE EventHandler
    Add(ClassType* inClass,
        ClassFuncTypeHelper<true, ClassType, RetType(Args..., std::decay_t<CommonArgs>...)>::Type inFunction,
        CommonArgs&&... inCommonArgs) {
        m_eventList.push_back(new Event<RetType(Args...)>());
        m_eventList.back()->Bind(inClass, inFunction, std::forward<CommonArgs>(inCommonArgs)...);
        return m_eventList.back()->GetHandler();
    }

    template<typename... CommonArgs>
    HUH_FORCE_INLINE EventHandler Add(RetType (*InFunction)(Args..., CommonArgs...), CommonArgs&&... commonArgs) {
        m_eventList.push_back(new Event<RetType(Args...)>());
        m_eventList.back()->Bind(InFunction, std::forward<CommonArgs>(commonArgs)...);
        return m_eventList.back()->GetHandler();
    }

    HUH_FORCE_INLINE void Remove(const EventHandler& handler) {
        for (size_t i = 0; i < m_eventList.size(); ++i) {
            if (m_eventList[i]->GetHandler() == handler) {
                delete m_eventList[i];
                m_eventList.erase(m_eventList.begin() + i);
            }
        }
    }

    HUH_FORCE_INLINE void ExecuteAll(Args... args) const {
        if (m_eventList.empty()) {
            return;
        }
        for (size_t i = m_eventList.size() - 1; i > 0; --i) {
            m_eventList[i]->Execute(args...);
        }
        m_eventList[0]->Execute(args...);
    }

    HUH_FORCE_INLINE void operator()(Args... args) const { ExecuteAll(args...); }

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
