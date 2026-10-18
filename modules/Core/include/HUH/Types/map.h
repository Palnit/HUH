#pragma once

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

    enum class SlotType : HUH::Uint8 {
        Empty = 0x80,
        Deleted = 0xFE,
    };

    Map() = default;

    void Update(const KeyType& key, const ValueType& value) {
        CheckAndReHash();

        auto [h1, h2] = GetHash(key);
        HUH_TLOG("H1: {:064b} H2: {:064b} ", h1, h2)
        const size_t mask = m_groups - 1;

        size_t group = static_cast<size_t>(h1) & mask;
        HUH_TLOG("Group {} Capacity {}", group, m_capacity);
        for (size_t i = 0; i < m_groups; ++i) {
            auto matches = MatchH2(group * GroupWidth, h2);
            HUH_TLOG("Slot results for H2 {:016b}", matches)

            auto empties = MatchEmpty(group * GroupWidth, h2);
            HUH_TLOG("Slot results for Empty {:016b}", empties)
        }
    }

private:
    HUH_NODISCARD HUH_CONSTEXPR_FORCE HUH::Pair<Uint64, Uint8> GetHash(const KeyType& key) {
        const Uint64 hash = HUH::Hash(key);
        HUH_TLOG("Hash: {:064b}", hash)
        static constexpr Uint64 mask = ((static_cast<Uint64>(1) << 57) - 1);
        return {hash & mask, static_cast<Uint8>(hash >> 57)};
    }

    HUH_FORCE_INLINE void CheckAndReHash() {
        if (m_control.IsEmpty()) {
            m_control.Resize(GroupWidth * 2, static_cast<Uint8>(SlotType::Empty));
            m_values.AddUnInitialized(GroupWidth * 2);
            m_keys.AddUnInitialized(GroupWidth * 2);
            m_capacity = GroupWidth * 2;
            m_groups = 2;
        }

        const auto limit = GetMaxLoad();
        // TODO Rehash and grow
    }

    HUH_CONSTEXPR_FORCE HUH::Uint16 MatchH2(size_t group, Uint8 h2) {
        const __m128i metadata = _mm_loadu_si128(reinterpret_cast<const __m128i*>(m_control.GetData() + group));

        const __m128i fingerprint = _mm_set1_epi8(static_cast<char>(h2));

        return static_cast<HUH::Uint16>(_mm_movemask_epi8(_mm_cmpeq_epi8(metadata, fingerprint)));
    }

    HUH_CONSTEXPR_FORCE HUH::Uint16 MatchEmpty(size_t group, Uint8 h2) {
        const __m128i metadata = _mm_loadu_si128(reinterpret_cast<const __m128i*>(m_control.GetData() + group));

        const __m128i fingerprint = _mm_set1_epi8(static_cast<char>(SlotType::Empty));

        return static_cast<HUH::Uint16>(_mm_movemask_epi8(_mm_cmpeq_epi8(metadata, fingerprint)));
    }

    HUH_NODISCARD HUH_CONSTEXPR_FORCE size_t GetMaxLoad() const { return m_capacity - m_capacity / 8; }

    HUH::Array<Uint8, AllocatorType> m_control;
    HUH::Array<KeyType, AllocatorType> m_keys;
    HUH::Array<ValueType, AllocatorType> m_values;
    size_t m_groups = 0;
    size_t m_capacity = 0;
    size_t m_size = 0;
    size_t m_tombstones = 0;
};

}// namespace HUH