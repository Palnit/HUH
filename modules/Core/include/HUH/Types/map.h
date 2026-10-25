#pragma once

#include "HUH/fastrange.h"

#include <HUH/Types/array.h>
#include <HUH/hash.h>

#include <HUH/Types/Allocators/base_allocator.h>
#include <HUH/Types/Allocators/malloc_allocator.h>

#include <pmmintrin.h>

namespace HUH {

template<typename KeyType, typename ValueType, HUH::Allocator AllocatorType = MallocAllocator>
class Map {

public:
    static constexpr size_t GroupWidth = 16;

    enum class SlotType : Uint8 {
        Empty = 0x80,
        Tombstone = 0xFE,
    };

    Map() = default;

    ~Map() {
        KeyType* key = m_keys;
        ValueType* value = m_values;
        for (const Uint8 *control = m_control, *controlEnd = m_control + m_capacity; control != controlEnd;
             ++control, ++key, ++value) {
            if (*control >= static_cast<Uint8>(SlotType::Empty)) {
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

    template<HUH::Constructable<KeyType> K, typename... V>
        requires(std::constructible_from<ValueType, V...>)
    void Update(K&& key, V&&... value) {
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

    ValueType* Find(const KeyType& key) {
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

    HUH_CONSTEXPR_FORCE void CheckAndReHash() {
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
            Update(std::move(*key), std::move(*value));
            DefaultDestruct(key);
            DefaultDestruct(value);
        }

        m_allocator.Deallocate(oldControl);
        m_allocator.Deallocate(oldValues);
        m_allocator.Deallocate(oldKeys);
    }

    HUH_NODISCARD HUH_CONSTEXPR_FORCE Uint16 MatchH2(const size_t group, const Uint8 h2) const {
        const __m128i metadata = _mm_loadu_si128(reinterpret_cast<const __m128i*>(m_control + group));

        const __m128i fingerprint = _mm_set1_epi8(static_cast<char>(h2));

        return static_cast<Uint16>(_mm_movemask_epi8(_mm_cmpeq_epi8(metadata, fingerprint)));
    }

    HUH_NODISCARD HUH_CONSTEXPR_FORCE Uint16 MatchEmpty(const size_t group) const {
        const __m128i metadata = _mm_loadu_si128(reinterpret_cast<const __m128i*>(m_control + group));

        const __m128i fingerprint = _mm_set1_epi8(static_cast<char>(SlotType::Empty));

        return static_cast<Uint16>(_mm_movemask_epi8(_mm_cmpeq_epi8(metadata, fingerprint)));
    }

    HUH_NODISCARD HUH_CONSTEXPR_FORCE Uint16 MatchTombstone(const size_t group) const {
        const __m128i metadata = _mm_loadu_si128(reinterpret_cast<const __m128i*>(m_control + group));

        const __m128i fingerprint = _mm_set1_epi8(static_cast<char>(SlotType::Tombstone));

        return static_cast<Uint16>(_mm_movemask_epi8(_mm_cmpeq_epi8(metadata, fingerprint)));
    }

    HUH_NODISCARD HUH_CONSTEXPR_FORCE size_t GetMaxLoad() const { return m_capacity - m_capacity * 0.125f; }

    template<auto T>
        requires(IsMemberPtr<T>)
    HUH_CONSTEXPR_FORCE void AllocateMemory(const size_t newMax) {
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