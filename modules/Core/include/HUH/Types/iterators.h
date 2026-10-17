#pragma once

#include "pair.h"

#include <HUH/concepts.h>

namespace HUH {

template<HUH::RangeIterable T>
    requires requires(T a) {
        { a.begin() };
    }
constexpr HUH_FORCE_INLINE auto BeginWrapper(T& a) {
    return a.begin();
}

template<HUH::RangeIterable T>
    requires requires(T a) {
        { begin(a) };
    }
constexpr HUH_FORCE_INLINE auto BeginWrapper(T& a) {
    return begin(a);
}

template<HUH::RangeIterable T>
    requires requires(T& a) {
        { a.end() };
    }
constexpr HUH_FORCE_INLINE auto EndWrapper(T& a) {
    return a.end();
}

template<HUH::RangeIterable T>
    requires requires(T& a) {
        { end(a) };
    }
constexpr HUH_FORCE_INLINE auto EndWrapper(T& a) {
    return end(a);
}

template<HUH::ForwardIterator T>
HUH_NODISCARD HUH_FORCE_INLINE size_t Distance(T first, T last) {
    if constexpr (requires {
                      { last - first } -> HUH::Same<decltype(last - first)>;
                  }) {
        return static_cast<size_t>(last - first);
    } else {
        size_t count = 0;

        while (first != last) {
            ++first;
            ++count;
        }

        return count;
    }
}

template<HUH::ForwardIterator T>
HUH_FORCE_INLINE void MoveIterator(T& first, T toMove) {

    if constexpr (requires(T itType, size_t advance) {
                      { itType += advance } -> HUH::Same<T&>;
                  }) {
        auto value = HUH::Distance(first, toMove);
        first += value;
    } else {

        while (first != toMove) {
            ++first;
        }
    }
}

template<HUH::ForwardIterator T, HUH::Aritmatic N>
HUH_FORCE_INLINE void MoveIterator(T& first, N toMove) {

    if constexpr (requires {
                      { first += toMove } -> HUH::Same<T&>;
                  }) {
        first += toMove;
    } else {
        for (N i = 0; i < toMove; ++i) {
            ++first;
        }
    }
}

template<HUH::Aritmatic T>
class CountIterator {
public:
    template<HUH::Aritmatic T2>
    friend class CountIterator;

    explicit CountIterator(T start, T stepSize = 1) : m_current(start), m_stepSize(stepSize) {}

    HUH_NODISCARD HUH_FORCE_INLINE const T& operator*() const { return m_current; }

    HUH_NODISCARD HUH_FORCE_INLINE T* operator->() const { return &m_current; }

    HUH_NODISCARD HUH_FORCE_INLINE bool operator==(const CountIterator& other) const {
        return m_current == other.m_current && m_stepSize == other.m_stepSize && m_end == other.m_end;
    }

    HUH_NODISCARD HUH_FORCE_INLINE bool operator!=(const CountIterator& other) const { return !operator==(other); }

    HUH_FORCE_INLINE CountIterator& operator++() {
        m_current += m_stepSize;
        return *this;
    }

    HUH_FORCE_INLINE CountIterator operator++(int) {
        CountIterator temp = *this;
        this->operator++();
        return temp;
    }

    template<HUH::Aritmatic T2>
    friend HUH_FORCE_INLINE CountIterator<T2>& begin(CountIterator<T2>& it) {
        return it;
    }

    template<HUH::Aritmatic T2>
    friend HUH_FORCE_INLINE CountIterator<T2> end(CountIterator<T2> it) {
        it.m_end = true;
        return it;
    }

private:
    T m_current;
    T m_stepSize;
    bool m_end = false;
};

template<typename T>
    requires HUH::RangeIterable<std::remove_reference_t<T>>
class CycleIterator {
public:
    template<typename T2>
        requires HUH::RangeIterable<std::remove_reference_t<T2>>
    friend class CycleIterator;

    explicit CycleIterator(T&& iterable)
        : m_iterable(std::forward<T>(iterable)),
          m_begin_it(HUH::BeginWrapper(m_iterable)),
          m_end_it(HUH::EndWrapper(m_iterable)) {}

    CycleIterator(CycleIterator& other)
        : m_iterable(other.m_iterable),
          m_begin_it(HUH::BeginWrapper(m_iterable)),
          m_end_it(HUH::EndWrapper(m_iterable)),
          m_end(other.m_end) {
        HUH::MoveIterator(m_begin_it, HUH::Distance(HUH::BeginWrapper(other.m_iterable), other.m_begin_it));
    }

    HUH_FORCE_INLINE CycleIterator& operator=(const CycleIterator& other) {
        m_iterable = other.m_iterable;
        m_begin_it = HUH::BeginWrapper(m_iterable);
        m_end_it = HUH::EndWrapper(m_iterable);
        m_end = other.m_end;

        HUH::MoveIterator(m_begin_it, HUH::Distance(HUH::BeginWrapper(other.m_iterable), other.m_begin_it));
        return *this;
    }

    CycleIterator(CycleIterator&& other) noexcept : m_begin_it(other.m_begin_it), m_end_it(other.m_end_it) {
        size_t toEnd = HUH::Distance(HUH::BeginWrapper(other.m_iterable), other.m_begin_it);
        m_iterable = std::move(other.m_iterable);
        m_begin_it = HUH::BeginWrapper(m_iterable);
        m_end_it = HUH::EndWrapper(m_iterable);
        HUH::MoveIterator(m_begin_it, toEnd);
        m_end = other.m_end;
    }

    HUH_FORCE_INLINE CycleIterator& operator=(CycleIterator&& other) noexcept {
        size_t toEnd = HUH::Distance(HUH::BeginWrapper(other.m_iterable), other.m_begin_it);
        m_iterable = std::move(other.m_iterable);
        m_begin_it = HUH::BeginWrapper(m_iterable);
        m_end_it = HUH::EndWrapper(m_iterable);
        HUH::MoveIterator(m_begin_it, toEnd);
        m_end = other.m_end;
        return *this;
    }

    HUH_NODISCARD HUH_FORCE_INLINE auto& operator*() const { return *m_begin_it; }

    HUH_NODISCARD HUH_FORCE_INLINE auto* operator->() const { return std::to_address(m_begin_it); }

    HUH_NODISCARD HUH_FORCE_INLINE bool operator==(const CycleIterator& other) const {
        return m_iterable == other.m_iterable && m_begin_it == other.m_begin_it && m_end_it == other.m_end_it
            && m_end == other.m_end;
    }

    HUH_NODISCARD HUH_FORCE_INLINE bool operator!=(const CycleIterator& other) const { return !operator==(other); }

    HUH_FORCE_INLINE CycleIterator& operator++() {
        ++m_begin_it;
        if (m_begin_it == m_end_it) {
            m_begin_it = HUH::BeginWrapper(m_iterable);
        }
        return *this;
    }

    HUH_FORCE_INLINE CycleIterator operator++(int) {
        CountIterator temp = *this;
        this->operator++();
        return temp;
    }

    template<typename T2>
    friend HUH_FORCE_INLINE CycleIterator<T2>& begin(CycleIterator<T2>& it) {
        return it;
    }

    template<typename T2>
    friend HUH_FORCE_INLINE CycleIterator<T2> end(CycleIterator<T2> it) {
        it.m_end = true;
        return it;
    }

private:
    using Type = std::conditional_t<std::is_lvalue_reference_v<T>, T, std::remove_cvref_t<T>>;
    Type m_iterable;
    decltype(HUH::BeginWrapper(m_iterable)) m_begin_it;
    decltype(HUH::EndWrapper(m_iterable)) m_end_it;
    bool m_end = false;
};

template<typename T>
    requires HUH::RangeIterable<std::remove_reference_t<T>>
CycleIterator(T&&) -> CycleIterator<T>;

template<typename T>
    requires HUH::RangeIterable<std::remove_reference_t<T>>
class EnumerateIterator {
public:
    template<typename T2>
        requires HUH::RangeIterable<std::remove_reference_t<T2>>
    friend class EnumerateIterator;

    explicit EnumerateIterator(T&& iterable)
        : m_iterable(std::forward<T>(iterable)),
          m_current(0),
          m_current_it(HUH::BeginWrapper(m_iterable)) {}

    EnumerateIterator(EnumerateIterator& other)
        : m_iterable(other.m_iterable),
          m_current(other.m_current),
          m_current_it(HUH::BeginWrapper(other.m_current_it)) {
        MoveIterator(m_current_it, m_current);
    }

    HUH_FORCE_INLINE EnumerateIterator& operator=(const EnumerateIterator& other) {
        m_iterable = other.m_iterable;
        m_current = other.m_current;
        m_current_it = HUH::BeginWrapper(m_iterable);
        MoveIterator(m_current_it, m_current);
        return *this;
    }

    EnumerateIterator(EnumerateIterator&& other) noexcept
        : m_iterable(std::move(other.m_iterable)),
          m_current(other.m_current),
          m_current_it(HUH::BeginWrapper(other.m_current_it)) {

        MoveIterator(m_current_it, m_current);
    }

    HUH_FORCE_INLINE EnumerateIterator& operator=(EnumerateIterator&& other) noexcept {
        m_iterable = std::move(other.m_iterable);
        m_current = other.m_current;
        m_current_it = HUH::BeginWrapper(m_iterable);
        MoveIterator(m_current_it, m_current);
        return *this;
    }

    HUH_NODISCARD HUH_FORCE_INLINE auto operator*() const {
        return HUH::Pair<size_t, decltype(*m_current_it)>(m_current, *m_current_it);
    }

    HUH_NODISCARD HUH_FORCE_INLINE bool operator==(const EnumerateIterator& other) const {
        return m_current_it == other.m_current_it;
    }

    HUH_NODISCARD HUH_FORCE_INLINE bool operator!=(const EnumerateIterator& other) const { return !operator==(other); }

    HUH_FORCE_INLINE EnumerateIterator& operator++() {
        ++m_current;
        ++m_current_it;
        return *this;
    }

    HUH_FORCE_INLINE EnumerateIterator operator++(int) {
        EnumerateIterator temp = *this;
        this->operator++();
        return temp;
    }

    template<typename T2>
    friend HUH_FORCE_INLINE EnumerateIterator<T2>& begin(EnumerateIterator<T2>& it) {
        return it;
    }

    template<typename T2>
    friend HUH_FORCE_INLINE EnumerateIterator<T2> end(EnumerateIterator<T2> it) {
        it.m_current_it = HUH::EndWrapper(it.m_iterable);
        return it;
    }

private:
    using Type = std::conditional_t<std::is_lvalue_reference_v<T>, T, std::remove_cvref_t<T>>;
    Type m_iterable;
    size_t m_current;
    decltype(HUH::BeginWrapper(m_iterable)) m_current_it;
};

template<typename T>
    requires HUH::RangeIterable<std::remove_reference_t<T>>
EnumerateIterator(T&&) -> EnumerateIterator<T>;

}// namespace HUH