#pragma once

#include <HUH/fastrange.h>

#include <HUH/Types/array.h>
#include <HUH/Types/pair.h>
#include <HUH/hash.h>

#include <HUH/Types/Allocators/base_allocator.h>
#include <HUH/Types/Allocators/malloc_allocator.h>

namespace HUH {

template<typename T>
concept Hashable = requires(const T& a) {
    { Hash(a) } -> HUH::Convertable<Uint64>;
} && HUH::Comparable<T>;

template<HUH::Hashable KeyType, typename ValueType, HUH::Allocator AllocatorType = MallocAllocator>
class Map {

public:
    static constexpr size_t GroupWidth = 16;

    enum class SlotType : Uint8 {
        Empty = 0x80,
        Tombstone = 0xFE,
    };

    template<bool Reverse>
    friend class Iterator;

    template<HUH::Hashable KeyTypeIt, typename ValueTypeIt>
    class Iterator {
    public:
        explicit Iterator(Uint8* control, KeyTypeIt* keys, ValueTypeIt* values, size_t capacity, bool end = false)
            : m_control(control),
              m_keys(keys),
              m_values(values),
              m_pair(end ? *(--m_keys) : *m_keys, end ? *(--m_values) : *m_values),
              m_capacity(capacity) {
            while (m_size != m_capacity
                   && (*m_control == static_cast<Uint8>(Map::SlotType::Empty)
                       || *m_control == static_cast<Uint8>(Map::SlotType::Tombstone))) {
                ++m_control;
                ++m_keys;
                ++m_values;
                ++m_size;
            }
        }

        HUH_NODISCARD HUH_FORCE_INLINE const HUH::Pair<const KeyTypeIt&, ValueTypeIt&>& operator*() {
            if (m_changed) {
                m_pair.~Pair();
                new (&m_pair) HUH::Pair<const KeyTypeIt&, ValueTypeIt&>{*m_keys, *m_values};
                m_changed = false;
            }
            return m_pair;
        }

        HUH_NODISCARD HUH_FORCE_INLINE const HUH::Pair<const KeyTypeIt&, ValueTypeIt&>* operator->() {
            if (m_changed) {
                m_pair.~Pair();
                new (&m_pair) HUH::Pair<const KeyTypeIt&, ValueTypeIt&>{*m_keys, *m_values};
                m_changed = false;
            }
            return &m_pair;
        }

        HUH_FORCE_INLINE Iterator& operator++() {
            do {
                ++m_control;
                ++m_keys;
                ++m_values;
                ++m_size;
            } while (m_size != m_capacity
                     && (*m_control == static_cast<Uint8>(Map::SlotType::Empty)
                         || *m_control == static_cast<Uint8>(Map::SlotType::Tombstone)));
            m_changed = true;
            return *this;
        }

        HUH_FORCE_INLINE Iterator operator++(int) {
            Iterator temp = *this;
            this->operator++();
            return temp;
        }

        HUH_FORCE_INLINE Iterator& operator--() {
            do {
                --m_control;
                --m_keys;
                --m_values;
                --m_size;
            } while (m_size != 0
                     && (*m_control == static_cast<Uint8>(Map::SlotType::Empty)
                         || *m_control == static_cast<Uint8>(Map::SlotType::Tombstone)));
            m_changed = true;
            return *this;
        }

        HUH_FORCE_INLINE Iterator operator--(int) {
            Iterator temp = *this;
            this->operator--();
            return temp;
        }

        HUH_NODISCARD HUH_FORCE_INLINE bool operator==(const Iterator& rhs) const {
            return m_control == rhs.m_control && m_keys == rhs.m_keys && m_values == rhs.m_values;
        }

        HUH_NODISCARD HUH_FORCE_INLINE bool operator!=(const Iterator& rhs) const { return !(*this == rhs); }

    private:
        Uint8* m_control = nullptr;
        KeyTypeIt* m_keys = nullptr;
        ValueTypeIt* m_values = nullptr;
        HUH::Pair<const KeyTypeIt&, ValueTypeIt&> m_pair{};
        size_t m_capacity = 0;
        size_t m_size = 0;
        bool m_changed = true;
    };

    explicit Map(AllocatorType&& allocator = MallocAllocator()) : m_allocator(allocator) {}

    ~Map() {
        KeyType* key = m_keys;
        ValueType* value = m_values;
        for (const Uint8 *control = m_control, *controlEnd = m_control + m_capacity; control != controlEnd;
             ++control, ++key, ++value) {
            if (*control == static_cast<Uint8>(SlotType::Empty)) {
                continue;
            }
            DefaultDestruct(key);
            DefaultDestruct(value);
        }

        DefaultDestruct(m_control, m_capacity);
        m_allocator.Deallocate(m_control);
        m_allocator.Deallocate(m_values);
        m_allocator.Deallocate(m_keys);
    };

    using ForwardIterator = Iterator<KeyType, ValueType>;
    using ConstForwardIterator = Iterator<KeyType, const ValueType>;

    ForwardIterator begin() { return ForwardIterator(m_control, m_keys, m_values, m_capacity); }
    ConstForwardIterator begin() const { return ConstForwardIterator(m_control, m_keys, m_values, m_capacity); }
    ForwardIterator end() {
        return ForwardIterator(m_control + m_capacity, m_keys + m_capacity, m_values + m_capacity, m_capacity, true);
    }

    template<HUH::Constructable<KeyType> K, typename... V>
        requires(std::constructible_from<ValueType, V...>)
    void Add(K&& key, V&&... value) {
        CheckAndReHash();

        auto [h1, h2] = GetHash(key);
        size_t group = HUH::FastRange(h1 << 7, m_groups);
        for (size_t i = 0; i < m_groups; ++i) {
            const auto offset = group * GroupWidth;
            auto matches = MatchH2(offset, h2);
            while (matches != 0) {
                const size_t lane = std::countr_zero(matches);

                const size_t index = offset + lane;

                if (m_keys[index] == key) {
                    DefaultDestruct(m_values + index);
                    (void)new (m_values + index) ValueType(std::forward<V>(value)...);
                    return;
                }

                matches &= matches - 1;
            }

            const auto empties = MatchEmpty(group * GroupWidth);
            if (empties != 0) {
                const size_t lane = std::countr_zero(empties);
                const size_t index = offset + lane;
                m_control[index] = h2;
                ++m_size;
                (void)new (m_keys + index) KeyType(std::forward<K>(key));
                (void)new (m_values + index) ValueType(std::forward<V>(value)...);
                return;
            }

            ++group;
            if (group >= m_groups) {
                group = 0;
            }
        }
    }

    template<HUH::Constructable<KeyType> K, HUH::Constructable<ValueType> V>
        requires(HUH::DefaultConstructable<ValueType>)
    HUH_NODISCARD HUH_FORCE_INLINE ValueType& FindOrAdd(K&& key, V&& value = ValueType()) {
        return FindOrAdd(std::forward<K>(key), std::forward<V>(value));
    }

    template<HUH::Constructable<KeyType> K, typename... V>
        requires(std::constructible_from<ValueType, V...>)
    HUH_NODISCARD HUH_FORCE_INLINE ValueType& FindOrAdd(K&& key, V&&... value) {
        CheckAndReHash();

        auto [h1, h2] = GetHash(key);
        size_t group = HUH::FastRange(h1 << 7, m_groups);
        for (size_t i = 0; i < m_groups; ++i) {
            const auto offset = group * GroupWidth;
            auto matches = MatchH2(offset, h2);
            while (matches != 0) {
                const size_t lane = std::countr_zero(matches);

                const size_t index = offset + lane;

                if (m_keys[index] == key) {
                    return *(m_values + index);
                }

                matches &= matches - 1;
            }

            const auto empties = MatchEmpty(group * GroupWidth);
            if (empties != 0) {
                const size_t lane = std::countr_zero(empties);
                const size_t index = offset + lane;
                m_control[index] = h2;
                ++m_size;
                (void)new (m_keys + index) KeyType(std::forward<K>(key));
                (void)new (m_values + index) ValueType(std::forward<V>(value)...);
                return *(m_values + index);
            }

            ++group;
            if (group >= m_groups) {
                group = 0;
            }
        }
        std::unreachable();
    }

    template<HUH::Constructable<KeyType> K>
    HUH_NODISCARD HUH_FORCE_INLINE ValueType* Find(K&& key) {
        if (IsEmpty()) {
            return nullptr;
        }

        auto [h1, h2] = GetHash(key);
        size_t group = HUH::FastRange(h1 << 7, m_groups);
        for (size_t i = 0; i < m_groups; ++i) {
            const auto offset = group * GroupWidth;
            auto matches = MatchH2(offset, h2);
            while (matches != 0) {
                const size_t lane = std::countr_zero(matches);

                const size_t index = offset + lane;

                if (m_keys[index] == key) {
                    return m_values + index;
                }

                matches &= matches - 1;
            }

            ++group;
            if (group >= m_groups) {
                group = 0;
            }
        }

        return nullptr;
    }

    template<HUH::Constructable<KeyType> K>
    HUH_FORCE_INLINE bool Remove(K&& key) {
        auto [h1, h2] = GetHash(key);
        size_t group = HUH::FastRange(h1 << 7, m_groups);
        for (size_t i = 0; i < m_groups; ++i) {
            const auto offset = group * GroupWidth;
            auto matches = MatchH2(offset, h2);
            while (matches != 0) {
                const size_t lane = std::countr_zero(matches);

                const size_t index = offset + lane;

                if (m_keys[index] == key) {
                    if (MatchEmpty(offset) != 0) {
                        m_control[index] = static_cast<Uint8>(SlotType::Empty);
                        DefaultDestruct(m_keys + index);
                        DefaultDestruct(m_values + index);
                        --m_size;
                        return true;
                    }
                    m_control[index] = static_cast<Uint8>(SlotType::Tombstone);
                    ++m_tombstones;
                    return true;
                }

                matches &= matches - 1;
            }

            ++group;
            if (group >= m_groups) {
                group = 0;
            }
        }
        return false;
    }

    template<HUH::Constructable<KeyType> K>
    HUH_NODISCARD HUH_FORCE_INLINE bool Contains(K&& key) {
        if (Find(std::forward<K>(key))) {
            return true;
        }
        return false;
    }

    template<HUH::Constructable<KeyType> K>
    HUH_NODISCARD HUH_FORCE_INLINE ValueType& operator[](K&& key) {
        return FindOrAdd(std::forward<K>(key));
    }

    HUH_NODISCARD HUH_CONSTEXPR_FORCE bool IsEmpty() const {
        if (m_control == nullptr) {
            return true;
        }
        return false;
    }

private:
    HUH_NODISCARD HUH_CONSTEXPR_FORCE HUH::Pair<const Uint64, const Uint8> GetHash(const KeyType& key) {
        const Uint64 hash = HUH::Hash(key);
        static constexpr Uint64 mask = ((static_cast<Uint64>(1) << 57) - 1);
        return {hash & mask, static_cast<Uint8>(hash >> 57)};
    }

    HUH_FORCE_INLINE void CheckAndReHash() {
        if (m_control == nullptr) {
            AllocateMemory<&Map::m_control>(GroupWidth);
            std::memset(m_control, static_cast<Uint8>(SlotType::Empty), sizeof(Uint8) * GroupWidth);
            AllocateMemory<&Map::m_values>(GroupWidth);
            AllocateMemory<&Map::m_keys>(GroupWidth);
            m_capacity = GroupWidth;
            m_groups = 1;
        }

        const auto limit = GetMaxLoad();
        if (m_tombstones + m_size + 1 < limit) {
            return;
        }
        auto oldCapacity = m_capacity;
        m_capacity *= 2;
        m_groups *= 2;

        auto oldControl = m_control;
        auto oldValues = m_values;
        auto oldKeys = m_keys;

        AllocateMemory<&Map::m_control>(m_capacity);
        std::memset(m_control, static_cast<Uint8>(SlotType::Empty), sizeof(Uint8) * m_capacity);
        AllocateMemory<&Map::m_values>(m_capacity);
        AllocateMemory<&Map::m_keys>(m_capacity);

        m_size = 0, m_tombstones = 0;
        KeyType* key = oldKeys;
        ValueType* value = oldValues;
        for (const Uint8 *control = oldControl, *controlEnd = oldControl + oldCapacity; control != controlEnd;
             ++control, ++key, ++value) {
            if (*control >= static_cast<Uint8>(SlotType::Empty)) {
                continue;
            }
            Add(std::move(*key), std::move(*value));
            DefaultDestruct(key);
            DefaultDestruct(value);
        }

        m_allocator.Deallocate(oldControl);
        m_allocator.Deallocate(oldValues);
        m_allocator.Deallocate(oldKeys);
    }

    HUH_NODISCARD HUH_FORCE_INLINE Uint16 MatchH2(const size_t group, const Uint8 h2) const {
        const __m128i metadata = _mm_loadu_si128(reinterpret_cast<const __m128i*>(m_control + group));

        const __m128i fingerprint = _mm_set1_epi8(static_cast<char>(h2));

        return static_cast<Uint16>(_mm_movemask_epi8(_mm_cmpeq_epi8(metadata, fingerprint)));
    }

    HUH_NODISCARD HUH_FORCE_INLINE Uint16 MatchEmpty(const size_t group) const {
        const __m128i metadata = _mm_loadu_si128(reinterpret_cast<const __m128i*>(m_control + group));

        const __m128i fingerprint = _mm_set1_epi8(static_cast<char>(SlotType::Empty));

        return static_cast<Uint16>(_mm_movemask_epi8(_mm_cmpeq_epi8(metadata, fingerprint)));
    }

    HUH_NODISCARD HUH_FORCE_INLINE Uint16 MatchTombstone(const size_t group) const {
        const __m128i metadata = _mm_loadu_si128(reinterpret_cast<const __m128i*>(m_control + group));

        const __m128i fingerprint = _mm_set1_epi8(static_cast<char>(SlotType::Tombstone));

        return static_cast<Uint16>(_mm_movemask_epi8(_mm_cmpeq_epi8(metadata, fingerprint)));
    }

    HUH_NODISCARD HUH_CONSTEXPR_FORCE size_t GetMaxLoad() const { return m_capacity - m_capacity * 0.125f; }

    template<auto T>
        requires(IsMemberPtr<T>)
    HUH_FORCE_INLINE void AllocateMemory(const size_t newMax) {
        using Type = std::remove_pointer_t<typename ClassMemberTypeHelper<T>::Type>;
        this->*T = static_cast<Type*>(m_allocator.Allocate(sizeof(Type) * newMax));
    }

    Uint8* m_control = nullptr;
    KeyType* m_keys = nullptr;
    ValueType* m_values = nullptr;
    size_t m_groups = 0;
    size_t m_capacity = 0;
    size_t m_tombstones = 0;
    size_t m_size = 0;
    [[no_unique_address]] AllocatorType m_allocator;
};

}// namespace HUH