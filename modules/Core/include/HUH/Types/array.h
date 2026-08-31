#pragma once

#include <HUH/Types/Allocators/base_allocator.h>
#include <HUH/Types/Allocators/malloc_allocator.h>
#include <HUH/memory_movement.h>
#include <HUH/types.h>

#include <cassert>
#include <cstring>

#include <utility>

namespace HUH {

template<typename Type, Allocator AllocatorType = MallocAllocator>
class Array {
public:
    template<typename IterType, bool Reverse>
    class Iterator {
    public:
        explicit Iterator(IterType* ptr) : m_ptr(ptr) {}

        HUH_NODISCARD HUH_FORCE_INLINE IterType& operator*() const {
            if constexpr (Reverse) {
                return *(m_ptr - 1);
            } else {
                return *m_ptr;
            }
        }

        HUH_NODISCARD HUH_FORCE_INLINE IterType* operator->() const {
            if constexpr (Reverse) {
                return (m_ptr - 1);
            } else {
                return m_ptr;
            }
        }

        HUH_FORCE_INLINE Iterator& operator++() {
            if constexpr (Reverse) {
                --m_ptr;
            } else {
                ++m_ptr;
            }
            return *this;
        }

        HUH_FORCE_INLINE Iterator operator++(int) {
            Iterator temp = *this;
            this->operator++();
            return temp;
        }

        HUH_FORCE_INLINE Iterator& operator--() {
            if constexpr (Reverse) {
                ++m_ptr;
            } else {
                --m_ptr;
            }
            return *this;
        }

        HUH_FORCE_INLINE Iterator operator--(int) {
            Iterator temp = *this;
            this->operator--();
            return temp;
        }

        HUH_NODISCARD HUH_FORCE_INLINE bool operator==(const Iterator& rhs) const { return m_ptr == rhs.m_ptr; }
        HUH_NODISCARD HUH_FORCE_INLINE bool operator!=(const Iterator& rhs) const { return m_ptr != rhs.m_ptr; }

        HUH_FORCE_INLINE Iterator& operator+=(size_t index) {
            m_ptr += index;
            return *this;
        }

        HUH_FORCE_INLINE Iterator& operator-=(size_t index) {
            m_ptr -= index;
            return *this;
        }

        HUH_FORCE_INLINE Iterator operator+(size_t index) const {
            Iterator temp = *this;
            temp += index;
            return temp;
        }

        HUH_FORCE_INLINE Iterator operator-(size_t index) const {
            Iterator temp = *this;
            temp -= index;
            return temp;
        }

    private:
        IterType* m_ptr;
    };

    Array() noexcept : m_size(0), m_data(nullptr) {};
    explicit Array(const size_t count) { AddDefaultConstructed(count); }
    Array(std::initializer_list<Type> initializerList) {
        CopyPtrDataToEmpty(initializerList.begin(), initializerList.size());
        m_size = initializerList.size();
    }
    Array(const size_t count, const Type& inElement) {
        AddDefaultConstructed(count);
        for (auto& element : *this) {
            element = inElement;
        }
    }
    Array(const Array& other) noexcept {
        CopyPtrDataToEmpty(other.m_data, other.m_size);
        m_size = other.m_size;
    }
    Array(Array&& other) noexcept
        : m_size(other.m_size),
          m_max(other.m_max),
          m_data(std::move(other.m_data)),
          m_allocator(std::move(other.m_allocator)) {
        other.m_data = nullptr;
        other.m_max = 0;
        other.m_size = 0;
    }

    Array& operator=(std::initializer_list<Type> initializerList) noexcept {
        DefaultDestruct(m_data, m_size);
        m_size = 0;
        m_max = 0;
        CopyPtrDataToEmpty(initializerList.begin(), initializerList.size());
        m_size = initializerList.size();
        return *this;
    }

    Array& operator=(const Array& other) noexcept {
        if (this == other) {
            return *this;
        }
        DefaultDestruct(m_data, m_size);
        m_size = 0;
        m_max = 0;
        CopyPtrDataToEmpty(other.m_data, other.m_size);
        m_size = other.m_size;
        return *this;
    }

    Array& operator=(Array&& other) noexcept {
        DefaultDestruct(m_data, m_size);
        m_size = other.m_size;
        m_max = other.m_max;
        m_data = std::move(other.m_data);
        m_allocator = std::move(other.m_allocator);

        other.m_data = nullptr;
        other.m_max = 0;
        other.m_size = 0;
        return *this;
    }

    HUH_FORCE_INLINE bool operator==(const Array& other) const {
        return m_size == other.m_size && CompareArrays(m_data, other.m_data, m_size);
    }

    HUH_FORCE_INLINE bool operator!=(const Array& other) const { return !operator==(other); }

    ~Array() {
        DefaultDestruct(m_data, m_size);
        m_allocator.Deallocate(m_data);
    }

    using ForwardIterator = Iterator<Type, false>;
    using ConstForwardIterator = Iterator<const Type, false>;
    using BackwardIterator = Iterator<Type, true>;
    using ConstBackwardIterator = Iterator<const Type, true>;

    HUH_NODISCARD HUH_FORCE_INLINE ForwardIterator begin() { return ForwardIterator(m_data); }
    HUH_NODISCARD HUH_FORCE_INLINE ConstForwardIterator begin() const { return ConstForwardIterator(m_data); }
    HUH_NODISCARD HUH_FORCE_INLINE ForwardIterator end() { return ForwardIterator(m_data + m_size); }
    HUH_NODISCARD HUH_FORCE_INLINE ConstForwardIterator end() const { return ConstForwardIterator(m_data + m_size); }

    HUH_NODISCARD HUH_FORCE_INLINE BackwardIterator rbegin() { return BackwardIterator(m_data); }
    HUH_NODISCARD HUH_FORCE_INLINE ConstBackwardIterator rbegin() const { return ConstBackwardIterator(m_data); }
    HUH_NODISCARD HUH_FORCE_INLINE BackwardIterator rend() { return BackwardIterator(m_data + m_size); }
    HUH_NODISCARD HUH_FORCE_INLINE ConstBackwardIterator rend() const { return ConstBackwardIterator(m_data + m_size); }

    HUH_NODISCARD Type& operator[](size_t index) {
        assert(index < m_size);
        return m_data[index];
    }

    HUH_NODISCARD const Type& operator[](size_t index) const {
        assert(index < m_size);
        return m_data[index];
    }

    HUH_NODISCARD HUH_FORCE_INLINE size_t Size() const { return m_size; }
    HUH_NODISCARD HUH_FORCE_INLINE size_t size() const { return m_size; }

    HUH_NODISCARD HUH_FORCE_INLINE size_t Capacity() const { return m_max; }
    HUH_NODISCARD HUH_FORCE_INLINE size_t capacity() const { return m_max; }

    HUH_NODISCARD HUH_FORCE_INLINE Type* GetData() const { return m_data; }
    HUH_NODISCARD HUH_FORCE_INLINE Type* data() const { return m_data; }

    HUH_NODISCARD HUH_FORCE_INLINE bool IsEmpty() const { return m_size == 0; }
    HUH_NODISCARD HUH_FORCE_INLINE bool empty() const { return m_size == 0; }

    HUH_NODISCARD HUH_FORCE_INLINE Type& Back() { return m_data[m_size - 1]; }
    HUH_NODISCARD HUH_FORCE_INLINE const Type& Back() const { return m_data[m_size - 1]; }

    HUH_NODISCARD HUH_FORCE_INLINE Type& back() { return m_data[m_size - 1]; }
    HUH_NODISCARD HUH_FORCE_INLINE const Type& back() const { return m_data[m_size - 1]; }

    HUH_FORCE_INLINE void Clear() {
        DefaultDestruct(m_data, m_size);
        m_size = 0;
    }

    HUH_FORCE_INLINE void clear() {
        DefaultDestruct(m_data, m_size);
        m_size = 0;
    }

    HUH_NODISCARD HUH_FORCE_INLINE size_t NumBytes() const { return m_size * sizeof(Type); }

    HUH_FORCE_INLINE void ShrinkToSize() { Shrink(m_size); }

    HUH_FORCE_INLINE void Resize(const size_t newMax) {
        if (newMax > m_size) {
            AddDefaultConstructed(newMax - m_size);
        }
        if (m_size < newMax) {
            Shrink(newMax);
        }
    }

    HUH_FORCE_INLINE void Resize(const size_t newMax, const Type& inElement) {
        if (newMax > m_size) {
            AddDefaultConstructed(newMax - m_size);
            for (auto& element : *this) {
                element = inElement;
            }
        }
        if (m_size < newMax) {
            Shrink(newMax);
            for (auto& element : *this) {
                element = inElement;
            }
        }
    }

    HUH_FORCE_INLINE void Reserve(const size_t newMax) {
        if (newMax > m_max) {
            AddUnInitialized(newMax - m_max);
        }
    }

    void InsertUninitialized(const size_t index, const size_t count) {
        assert(index <= m_size);
        assert(count >= 1);

        m_max += count;
        auto tmp = static_cast<Type*>(m_allocator.Allocate(sizeof(Type) * m_max));
        DefaultMove<Type>(tmp, m_data, index);
        DefaultMove<Type>((tmp + index + count), (m_data + index), m_size - index);
        m_allocator.Deallocate(m_data);
        m_data = tmp;
        m_size += count;
    }

    HUH_FORCE_INLINE void AddUnInitialized(const size_t count) { FixedGrow(count); }

    HUH_FORCE_INLINE void AddDefaultConstructed(const size_t count) {
        if (m_size + count > m_max) {
            Grow(m_size + count);
        }
        DefaultConstruct<Type>(m_data + m_size, count);
        m_size += count;
    }

    template<typename... Args>
        requires(std::constructible_from<Type, Args...>)
    HUH_FORCE_INLINE size_t Emplace(Args&&... args) {
        if (m_size == m_max) {
            Grow(0);
        }

        auto ptr = m_data + m_size;
        (void)new (ptr) Type(std::forward<Args>(args)...);
        return m_size++;
    }

    HUH_FORCE_INLINE size_t Emplace(const Type& other) { return Emplace<const Type&>(other); }
    HUH_FORCE_INLINE size_t Emplace(Type&& other) { return Emplace<Type&&>(std::move(other)); }

    HUH_FORCE_INLINE void Emplace(const HUH::Array<Type>& other) {
        AddUnInitialized(other.size());

        auto ptr = m_data + m_size;
        for (auto& element : other) {
            (void)new (ptr) Type(element);
            ++ptr;
        }
        m_size += other.size();
    }

    HUH_FORCE_INLINE void Emplace(std::initializer_list<Type> initializerList) {
        AddUnInitialized(initializerList.size());
        auto ptr = m_data + m_size;
        for (auto& element : initializerList) {
            (void)new (ptr) Type(std::forward<decltype(element)>(element));
            ++ptr;
        }
        m_size += initializerList.size();
    }

    template<typename... Args>
    HUH_FORCE_INLINE void Insert(size_t index, Args&&... args) {
        InsertUninitialized(index, 1);
        auto ptr = m_data + index;
        (void)new (ptr) Type(std::forward<Args>(args)...);
    }

    HUH_FORCE_INLINE void Insert(size_t index, const Type& other) { Insert<const Type&>(other, index); }
    HUH_FORCE_INLINE void Insert(size_t index, Type&& other) { Insert<Type&&>(other, index); }

    HUH_FORCE_INLINE void Insert(const size_t index, std::initializer_list<Type> initializerList) {
        InsertUninitialized(index, initializerList.size());
        auto ptr = m_data + index;
        for (auto& element : initializerList) {
            (void)new (ptr) Type(std::forward<decltype(element)>(element));
            ++ptr;
        }
    }

    template<typename Key>
    HUH_NODISCARD size_t FindIndex(const Key& key) {
        for (const Type *element = m_data, *end = m_data + m_size; element != end; ++element) {
            if (*element == key) {
                return element - m_data;
            }
        }
        return -1;
    }

    template<typename Pred>
    HUH_NODISCARD size_t FindIndexByPred(Pred pred) {
        for (const Type *element = m_data, *end = m_data + m_size; element != end; ++element) {
            if (std::invoke(pred, *element)) {
                return element - m_data;
            }
        }
        return -1;
    }

private:
    template<typename OtherType>
    HUH_FORCE_INLINE void CopyPtrDataToEmpty(const OtherType* data, const size_t size) {
        Grow(size);
        DefaultCopy<Type>(m_data, data, size);
    }

    HUH_FORCE_INLINE void Grow(const size_t newMax) {
        size_t result = 4;
        if (m_max) {
            result = static_cast<size_t>(m_max * 1.5);
        } else if (newMax > result) {
            result = newMax;
        }
        ResizeAllocator(result);
    }

    HUH_FORCE_INLINE void FixedGrow(const size_t newMax) {
        assert(m_max >= newMax + m_max);

        ResizeAllocator(newMax);
    }

    HUH_FORCE_INLINE void Shrink(const size_t newMax) {
        if (m_size > newMax) {
            DefaultDestruct(m_data + newMax + 1, m_size - newMax);
            m_size = newMax;
        }
        ResizeAllocator(newMax);
    }

    void ResizeAllocator(const size_t newMax) {
        m_max = newMax;
        auto tmp = static_cast<Type*>(m_allocator.Allocate(sizeof(Type) * m_max));
        DefaultMove<Type>(tmp, m_data, m_size);
        m_allocator.Deallocate(m_data);
        m_data = tmp;
    }

    size_t m_size = 0;
    size_t m_max = 0;
    Type* m_data = nullptr;
    AllocatorType m_allocator;
};

}// namespace HUH