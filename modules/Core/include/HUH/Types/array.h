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

    HUH_FORCE_INLINE void Reserve(const size_t newMax) {
        if (newMax > m_max) {
            AddUnInitialized(newMax - m_max);
        }
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
    HUH_FORCE_INLINE size_t Emplace(Args&&... args) {
        if (m_size == m_max) {
            Grow(0);
        }

        auto ptr = m_data + m_size;
        (void)new (ptr) Type(std::forward<Args>(args)...);
        return m_size++;
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
        m_max = result;
        m_data = static_cast<Type*>(m_allocator.Reallocate(m_data, sizeof(Type) * result));
    }

    HUH_FORCE_INLINE void FixedGrow(const size_t newMax) {
        assert(m_max >= newMax + m_max);

        m_max = newMax + m_max;
        m_data = static_cast<Type*>(m_allocator.Reallocate(m_data, sizeof(Type) * m_max));
    }

    HUH_FORCE_INLINE void Shrink(const size_t newMax) {
        if (m_size > newMax) {
            DefaultDestruct(m_data + newMax + 1, m_size - newMax);
        }
        m_allocator.Reallocate(m_data, sizeof(Type) * newMax);
    }

    size_t m_size = 0;
    size_t m_max = 0;
    Type* m_data = nullptr;
    AllocatorType m_allocator;
};

}// namespace HUH