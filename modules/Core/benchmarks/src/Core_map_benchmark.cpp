#include <HUH/hash.h>// Hash declarations must precede custom HUH::Hash overloads.
#include <benchmark/benchmark.h>

#include <algorithm>
#include <array>
#include <compare>
#include <cstddef>
#include <cstdint>
#include <map>
#include <optional>
#include <random>
#include <string>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

// -----------------------------------------------------------------------------
// Benchmark data types
// -----------------------------------------------------------------------------

struct Simple {
    int a = 0;

    Simple() = default;
    explicit Simple(int value) : a(value) {}

    Simple& operator+=(const Simple& other) {
        a += other.a;
        return *this;
    }
};

class Complex {
public:
    Complex() : a(0), b(nullptr) {}
    explicit Complex(int value) : a(value), b(new Simple{1}) {}

    Complex(const Complex&) = delete;
    Complex& operator=(const Complex&) = delete;

    Complex(Complex&& other) noexcept : a(other.a), b(other.b) { other.b = nullptr; }

    Complex& operator=(Complex&& other) noexcept {
        if (this != &other) {
            delete b;
            a = other.a;
            b = other.b;
            other.b = nullptr;
        }
        return *this;
    }

    Complex& operator+=(const Complex& other) {
        a += other.a;
        return *this;
    }

    ~Complex() { delete b; }

private:
    int a;
    Simple* b;
};

struct LargeValue {
    std::array<std::uint64_t, 8> words{};

    LargeValue() = default;
    explicit LargeValue(int value) { words[0] = static_cast<std::uint64_t>(value); }

    LargeValue& operator+=(const LargeValue& rhs) {
        for (std::size_t i = 0; i < words.size(); ++i) {
            words[i] += rhs.words[i];
        }
        return *this;
    }
};

// A small, trivially copyable user-defined key.
struct SmallKey {
    std::uint32_t nameSpace = 0;
    std::uint32_t id = 0;

    friend bool operator==(const SmallKey&, const SmallKey&) = default;
    friend auto operator<=>(const SmallKey&, const SmallKey&) = default;
};

// A more expensive key: hashing/comparing it also touches an owned string.
struct CompositeKey {
    std::uint32_t tenant = 0;
    std::string name;
    std::uint64_t generation = 0;

    friend bool operator==(const CompositeKey&, const CompositeKey&) = default;
    friend auto operator<=>(const CompositeKey&, const CompositeKey&) = default;
};

// HUH::Map's Hashable concept requires HUH::Hash(key) and equality.
// For struct keys, each subsequent field is hashed using the previous field's
// hash as the seed. This gives deterministic field-combination without simply
// XORing the individual hashes together.
namespace HUH {

inline Uint64 Hash(const SmallKey& key, const size_t seed = 0) {
    const Uint64 first = HUH::Hash(key.nameSpace, seed);
    return HUH::Hash(key.id, static_cast<size_t>(first));
}

inline Uint64 Hash(const CompositeKey& key, const size_t seed = 0) {
    const Uint64 tenantHash = HUH::Hash(key.tenant, seed);
    const Uint64 nameHash = HUH::Hash(key.name, static_cast<size_t>(tenantHash));
    return HUH::Hash(key.generation, static_cast<size_t>(nameHash));
}

}// namespace HUH

#include <HUH/Types/map.h>// Adjust if your Map header uses a different path.

// std::unordered_map uses the same HUH hash implementation as HUH::Map so the
// comparison is about the table implementations, not unrelated hashers.
template<typename Key>
struct HuhHasher {
    std::size_t operator()(const Key& key) const { return static_cast<std::size_t>(HUH::Hash(key)); }
};

// -----------------------------------------------------------------------------
// Container selection and common operations
// -----------------------------------------------------------------------------

enum class Backend {
    StdMap,
    StdUnorderedMap,
    HuhMap,
};

template<Backend B, typename Key, typename Value>
struct ContainerSelector;

template<typename Key, typename Value>
struct ContainerSelector<Backend::StdMap, Key, Value> {
    using Type = std::map<Key, Value>;
};

template<typename Key, typename Value>
struct ContainerSelector<Backend::StdUnorderedMap, Key, Value> {
    using Type = std::unordered_map<Key, Value, HuhHasher<Key>>;
};

template<typename Key, typename Value>
struct ContainerSelector<Backend::HuhMap, Key, Value> {
    using Type = HUH::Map<Key, Value>;
};

template<Backend B, typename Key, typename Value>
using MapType = typename ContainerSelector<B, Key, Value>::Type;

template<typename Value>
static Value MakeValue(std::size_t index) {
    if constexpr (std::is_same_v<Value, int>) {
        return static_cast<int>(index);
    } else {
        return Value(static_cast<int>(index));
    }
}

template<typename Key>
static Key MakeKey(std::size_t index) {
    if constexpr (std::is_same_v<Key, int>) {
        return static_cast<int>(index);
    } else if constexpr (std::is_same_v<Key, std::string>) {
        return "key-" + std::to_string(index);
    } else if constexpr (std::is_same_v<Key, SmallKey>) {
        return SmallKey{
            static_cast<std::uint32_t>((index >> 16) & 0xFFFFu),
            static_cast<std::uint32_t>(index & 0xFFFFu),
        };
    } else if constexpr (std::is_same_v<Key, CompositeKey>) {
        return CompositeKey{
            static_cast<std::uint32_t>(index % 97),
            "customer-" + std::to_string(index),
            static_cast<std::uint64_t>(index % 31),
        };
    } else {
        static_assert(!sizeof(Key), "Add a MakeKey specialization for this key type.");
    }
}

template<Backend B, typename Key, typename Value>
static void InsertOrAssign(MapType<B, Key, Value>& map, const Key& key, Value value) {
    if constexpr (B == Backend::HuhMap) {
        // HUH::Map::Add has insert-or-assign behavior.
        map.Add(key, std::move(value));
    } else {
        map.insert_or_assign(key, std::move(value));
    }
}

template<Backend B, typename Key, typename Value>
static Value* FindValue(MapType<B, Key, Value>& map, const Key& key) {
    if constexpr (B == Backend::HuhMap) {
        return map.Find(key);
    } else {
        const auto it = map.find(key);
        return it == map.end() ? nullptr : &it->second;
    }
}

template<Backend B, typename Key, typename Value>
static Value& FindOrAddDefault(MapType<B, Key, Value>& map, const Key& key) {
    if constexpr (B == Backend::HuhMap) {
        return map.FindOrAdd(key);
    } else {
        return map.try_emplace(key).first->second;
    }
}

template<Backend B, typename Key, typename Value>
static Value& Subscript(MapType<B, Key, Value>& map, const Key& key) {
    return map[key];
}

template<Backend B, typename Key, typename Value>
static bool ContainsKey(MapType<B, Key, Value>& map, const Key& key) {
    if constexpr (B == Backend::HuhMap) {
        return map.Contains(key);
    } else {
        return map.contains(key);
    }
}

template<Backend B, typename Key, typename Value>
static bool EraseKey(MapType<B, Key, Value>& map, const Key& key) {
    if constexpr (B == Backend::HuhMap) {
        return map.Remove(key);
    } else {
        return map.erase(key) != 0;
    }
}

template<Backend B, typename Key, typename Value>
static void Populate(MapType<B, Key, Value>& map, const std::vector<Key>& keys) {
    for (std::size_t i = 0; i < keys.size(); ++i) {
        InsertOrAssign<B, Key, Value>(map, keys[i], MakeValue<Value>(i));
    }
}

static std::vector<std::size_t> MakeShuffledIndices(std::size_t count) {
    std::vector<std::size_t> indices(count);
    for (std::size_t i = 0; i < count; ++i) {
        indices[i] = i;
    }
    std::mt19937_64 rng(0xC001D00Du);
    std::shuffle(indices.begin(), indices.end(), rng);
    return indices;
}

// -----------------------------------------------------------------------------
// Benchmarks
// -----------------------------------------------------------------------------

// Fresh construction + insertion. Destruction is included in the measured
// iteration, just as it is for the vector benchmark's local container.
template<Backend B, typename Key, typename Value>
static void BM_Insert(benchmark::State& state) {
    const auto count = static_cast<std::size_t>(state.range(0));
    std::vector<Key> keys;
    keys.reserve(count);
    for (std::size_t i = 0; i < count; ++i) {
        keys.emplace_back(MakeKey<Key>(i));
    }

    for (auto _ : state) {
        MapType<B, Key, Value> map;
        Populate<B, Key, Value>(map, keys);
        benchmark::DoNotOptimize(&map);
        benchmark::ClobberMemory();
    }

    state.SetItemsProcessed(state.iterations() * static_cast<std::int64_t>(count));
}

// Insert-or-find with a default-constructed mapped value. The standard
// containers use try_emplace; HUH uses FindOrAdd(key) with an empty value pack.
template<Backend B, typename Key, typename Value>
static void BM_FindOrAddInsert(benchmark::State& state) {
    const auto count = static_cast<std::size_t>(state.range(0));
    std::vector<Key> keys;
    keys.reserve(count);
    for (std::size_t i = 0; i < count; ++i) {
        keys.emplace_back(MakeKey<Key>(i));
    }

    for (auto _ : state) {
        MapType<B, Key, Value> map;
        for (std::size_t i = 0; i < count; ++i) {
            Value& value = FindOrAddDefault<B, Key, Value>(map, keys[i]);
            value = MakeValue<Value>(i);
        }
        benchmark::DoNotOptimize(&map);
        benchmark::ClobberMemory();
    }

    state.SetItemsProcessed(state.iterations() * static_cast<std::int64_t>(count));
}

// Repeated FindOrAdd calls for keys that are already present.
template<Backend B, typename Key, typename Value>
static void BM_FindOrAddHit(benchmark::State& state) {
    const auto count = static_cast<std::size_t>(state.range(0));
    std::vector<Key> keys;
    keys.reserve(count);
    for (std::size_t i = 0; i < count; ++i) {
        keys.emplace_back(MakeKey<Key>(i));
    }
    const auto order = MakeShuffledIndices(count);

    MapType<B, Key, Value> map;
    Populate<B, Key, Value>(map, keys);

    for (auto _ : state) {
        Value total{};
        for (const auto index : order) {
            total += FindOrAddDefault<B, Key, Value>(map, keys[index]);
        }
        benchmark::DoNotOptimize(total);
    }

    state.SetItemsProcessed(state.iterations() * static_cast<std::int64_t>(count));
}

// The operator[] equivalent: default-insert each key, then mutate its value.
template<Backend B, typename Key, typename Value>
static void BM_SubscriptInsert(benchmark::State& state) {
    const auto count = static_cast<std::size_t>(state.range(0));
    std::vector<Key> keys;
    keys.reserve(count);
    for (std::size_t i = 0; i < count; ++i) {
        keys.emplace_back(MakeKey<Key>(i));
    }

    for (auto _ : state) {
        MapType<B, Key, Value> map;
        for (std::size_t i = 0; i < count; ++i) {
            Subscript<B, Key, Value>(map, keys[i]) = MakeValue<Value>(i);
        }
        benchmark::DoNotOptimize(&map);
        benchmark::ClobberMemory();
    }

    state.SetItemsProcessed(state.iterations() * static_cast<std::int64_t>(count));
}

// operator[] on keys that are already present.
template<Backend B, typename Key, typename Value>
static void BM_SubscriptHit(benchmark::State& state) {
    const auto count = static_cast<std::size_t>(state.range(0));
    std::vector<Key> keys;
    keys.reserve(count);
    for (std::size_t i = 0; i < count; ++i) {
        keys.emplace_back(MakeKey<Key>(i));
    }
    const auto order = MakeShuffledIndices(count);

    MapType<B, Key, Value> map;
    Populate<B, Key, Value>(map, keys);

    for (auto _ : state) {
        Value total{};
        for (const auto index : order) {
            total += Subscript<B, Key, Value>(map, keys[index]);
        }
        benchmark::DoNotOptimize(total);
    }

    state.SetItemsProcessed(state.iterations() * static_cast<std::int64_t>(count));
}

// Repeatedly updates an already populated map. This isolates the duplicate-key
// path in HUH::Map::Add and std::map/std::unordered_map::insert_or_assign.
template<Backend B, typename Key, typename Value>
static void BM_UpdateExisting(benchmark::State& state) {
    const auto count = static_cast<std::size_t>(state.range(0));
    std::vector<Key> keys;
    keys.reserve(count);
    for (std::size_t i = 0; i < count; ++i) {
        keys.emplace_back(MakeKey<Key>(i));
    }

    MapType<B, Key, Value> map;
    Populate<B, Key, Value>(map, keys);

    for (auto _ : state) {
        for (std::size_t i = 0; i < count; ++i) {
            InsertOrAssign<B, Key, Value>(map, keys[i], MakeValue<Value>(i + 1));
        }
        benchmark::DoNotOptimize(&map);
        benchmark::ClobberMemory();
    }

    state.SetItemsProcessed(state.iterations() * static_cast<std::int64_t>(count));
}

// Successful lookup in a deterministic shuffled key order.
template<Backend B, typename Key, typename Value>
static void BM_FindHit(benchmark::State& state) {
    const auto count = static_cast<std::size_t>(state.range(0));
    std::vector<Key> keys;
    keys.reserve(count);
    for (std::size_t i = 0; i < count; ++i) {
        keys.emplace_back(MakeKey<Key>(i));
    }
    const auto order = MakeShuffledIndices(count);

    MapType<B, Key, Value> map;
    Populate<B, Key, Value>(map, keys);

    for (auto _ : state) {
        std::size_t found = 0;
        for (const auto index : order) {
            Value* value = FindValue<B, Key, Value>(map, keys[index]);
            found += value != nullptr;
            benchmark::DoNotOptimize(value);
        }
        benchmark::DoNotOptimize(found);
    }

    state.SetItemsProcessed(state.iterations() * static_cast<std::int64_t>(count));
}

// Unsuccessful lookup. Missing keys are generated outside the timed loop.
template<Backend B, typename Key, typename Value>
static void BM_FindMiss(benchmark::State& state) {
    const auto count = static_cast<std::size_t>(state.range(0));
    std::vector<Key> missingKeys;
    missingKeys.reserve(count);
    for (std::size_t i = 0; i < count; ++i) {
        missingKeys.emplace_back(MakeKey<Key>(i + count + 0x10000u));
    }
    const auto order = MakeShuffledIndices(count);

    MapType<B, Key, Value> map;
    std::vector<Key> presentKeys;
    presentKeys.reserve(count);
    for (std::size_t i = 0; i < count; ++i) {
        presentKeys.emplace_back(MakeKey<Key>(i));
    }
    Populate<B, Key, Value>(map, presentKeys);

    for (auto _ : state) {
        std::size_t found = 0;
        for (const auto index : order) {
            Value* value = FindValue<B, Key, Value>(map, missingKeys[index]);
            found += value != nullptr;
            benchmark::DoNotOptimize(value);
        }
        benchmark::DoNotOptimize(found);
    }

    state.SetItemsProcessed(state.iterations() * static_cast<std::int64_t>(count));
}

// Contains is measured separately from Find so the API-level cost is visible.
template<Backend B, typename Key, typename Value>
static void BM_ContainsHit(benchmark::State& state) {
    const auto count = static_cast<std::size_t>(state.range(0));
    std::vector<Key> keys;
    keys.reserve(count);
    for (std::size_t i = 0; i < count; ++i) {
        keys.emplace_back(MakeKey<Key>(i));
    }
    const auto order = MakeShuffledIndices(count);

    MapType<B, Key, Value> map;
    Populate<B, Key, Value>(map, keys);

    for (auto _ : state) {
        std::size_t found = 0;
        for (const auto index : order) {
            found += ContainsKey<B, Key, Value>(map, keys[index]);
        }
        benchmark::DoNotOptimize(found);
    }

    state.SetItemsProcessed(state.iterations() * static_cast<std::int64_t>(count));
}

template<Backend B, typename Key, typename Value>
static void BM_ContainsMiss(benchmark::State& state) {
    const auto count = static_cast<std::size_t>(state.range(0));
    std::vector<Key> missingKeys;
    missingKeys.reserve(count);
    for (std::size_t i = 0; i < count; ++i) {
        missingKeys.emplace_back(MakeKey<Key>(i + count + 0x10000u));
    }
    const auto order = MakeShuffledIndices(count);

    std::vector<Key> presentKeys;
    presentKeys.reserve(count);
    for (std::size_t i = 0; i < count; ++i) {
        presentKeys.emplace_back(MakeKey<Key>(i));
    }
    MapType<B, Key, Value> map;
    Populate<B, Key, Value>(map, presentKeys);

    for (auto _ : state) {
        std::size_t found = 0;
        for (const auto index : order) {
            found += ContainsKey<B, Key, Value>(map, missingKeys[index]);
        }
        benchmark::DoNotOptimize(found);
    }

    state.SetItemsProcessed(state.iterations() * static_cast<std::int64_t>(count));
}

// Rebuild the map while timing is paused, then time only erasing all entries.
// Rebuilding is necessary because each iteration consumes the entries.
template<Backend B, typename Key, typename Value>
static void BM_EraseAll(benchmark::State& state) {
    const auto count = static_cast<std::size_t>(state.range(0));
    std::vector<Key> keys;
    keys.reserve(count);
    for (std::size_t i = 0; i < count; ++i) {
        keys.emplace_back(MakeKey<Key>(i));
    }
    const auto order = MakeShuffledIndices(count);

    std::optional<MapType<B, Key, Value>> map;
    for (auto _ : state) {
        state.PauseTiming();
        map.reset();
        map.emplace();
        Populate<B, Key, Value>(*map, keys);
        state.ResumeTiming();

        std::size_t erased = 0;
        for (const auto index : order) {
            erased += EraseKey<B, Key, Value>(*map, keys[index]);
        }
        benchmark::DoNotOptimize(erased);
        benchmark::DoNotOptimize(&*map);

        state.PauseTiming();
    }
    map.reset();

    state.SetItemsProcessed(state.iterations() * static_cast<std::int64_t>(count));
}

// Full sequential traversal. HUH iteration is intentionally not registered by
// default because the Iterator in the supplied implementation currently has
// invalid dereferences in its constructor and end() construction.
template<Backend B, typename Key, typename Value>
static void BM_Iteration(benchmark::State& state) {
    const auto count = static_cast<std::size_t>(state.range(0));
    std::vector<Key> keys;
    keys.reserve(count);
    for (std::size_t i = 0; i < count; ++i) {
        keys.emplace_back(MakeKey<Key>(i));
    }

    MapType<B, Key, Value> map;
    Populate<B, Key, Value>(map, keys);

    for (auto _ : state) {
        Value total{};
        for (auto&& [key, value] : map) {
            (void)key;
            total += value;
        }
        benchmark::DoNotOptimize(total);
    }

    state.SetItemsProcessed(state.iterations() * static_cast<std::int64_t>(count));
}

// -----------------------------------------------------------------------------
// Registration
// -----------------------------------------------------------------------------

#define REGISTER_COMMON_MAP_BENCHMARKS(backend, keyType, valueType, maxRange)          \
    BENCHMARK_TEMPLATE(BM_Insert, backend, keyType, valueType)                         \
        ->RangeMultiplier(2)->Range(8, maxRange);                                      \
    BENCHMARK_TEMPLATE(BM_FindOrAddInsert, backend, keyType, valueType)                \
        ->RangeMultiplier(2)->Range(8, maxRange);                                      \
    BENCHMARK_TEMPLATE(BM_FindOrAddHit, backend, keyType, valueType)                   \
        ->RangeMultiplier(2)->Range(8, maxRange);                                      \
    BENCHMARK_TEMPLATE(BM_SubscriptInsert, backend, keyType, valueType)                \
        ->RangeMultiplier(2)->Range(8, maxRange);                                      \
    BENCHMARK_TEMPLATE(BM_SubscriptHit, backend, keyType, valueType)                   \
        ->RangeMultiplier(2)->Range(8, maxRange);                                      \
    BENCHMARK_TEMPLATE(BM_UpdateExisting, backend, keyType, valueType)                 \
        ->RangeMultiplier(2)->Range(8, maxRange);                                      \
    BENCHMARK_TEMPLATE(BM_FindHit, backend, keyType, valueType)                        \
        ->RangeMultiplier(2)->Range(8, maxRange);                                      \
    BENCHMARK_TEMPLATE(BM_FindMiss, backend, keyType, valueType)                       \
        ->RangeMultiplier(2)->Range(8, maxRange);                                      \
    BENCHMARK_TEMPLATE(BM_ContainsHit, backend, keyType, valueType)                    \
        ->RangeMultiplier(2)->Range(8, maxRange);                                      \
    BENCHMARK_TEMPLATE(BM_ContainsMiss, backend, keyType, valueType)                   \
        ->RangeMultiplier(2)->Range(8, maxRange)

#define REGISTER_ITERATION_BENCHMARK(backend, keyType, valueType, maxRange)             \
    BENCHMARK_TEMPLATE(BM_Iteration, backend, keyType, valueType)                      \
        ->RangeMultiplier(2)->Range(8, maxRange)

#define REGISTER_ERASE_BENCHMARK(backend, keyType, valueType, maxRange)                \
    BENCHMARK_TEMPLATE(BM_EraseAll, backend, keyType, valueType)                       \
        ->RangeMultiplier(2)->Range(8, maxRange)

#define REGISTER_STANDARD_ERASES(keyType, valueType, maxRange)                         \
    REGISTER_ERASE_BENCHMARK(Backend::StdMap, keyType, valueType, maxRange);            \
    REGISTER_ERASE_BENCHMARK(Backend::StdUnorderedMap, keyType, valueType, maxRange)

#define REGISTER_ALL_BACKENDS(keyType, valueType, maxRange)                             \
    REGISTER_COMMON_MAP_BENCHMARKS(Backend::StdMap, keyType, valueType, maxRange);      \
    REGISTER_COMMON_MAP_BENCHMARKS(Backend::StdUnorderedMap, keyType, valueType, maxRange); \
    REGISTER_COMMON_MAP_BENCHMARKS(Backend::HuhMap, keyType, valueType, maxRange);      \
    REGISTER_ITERATION_BENCHMARK(Backend::StdMap, keyType, valueType, maxRange);       \
    REGISTER_ITERATION_BENCHMARK(Backend::StdUnorderedMap, keyType, valueType, maxRange)

// Integer keys: inexpensive hashing and comparison.
REGISTER_ALL_BACKENDS(int, int, 8 << 15);
REGISTER_ALL_BACKENDS(int, Simple, 8 << 15);
REGISTER_ALL_BACKENDS(int, Complex, 8 << 12);
REGISTER_ALL_BACKENDS(int, LargeValue, 8 << 13);

// String keys: includes string hashing/comparison and key storage costs.
REGISTER_ALL_BACKENDS(std::string, int, 8 << 12);
REGISTER_ALL_BACKENDS(std::string, Simple, 8 << 12);
REGISTER_ALL_BACKENDS(std::string, Complex, 8 << 11);
REGISTER_ALL_BACKENDS(std::string, LargeValue, 8 << 11);

// Custom two-field key: hashes and compares two integer fields.
REGISTER_ALL_BACKENDS(SmallKey, int, 8 << 15);
REGISTER_ALL_BACKENDS(SmallKey, Simple, 8 << 15);
REGISTER_ALL_BACKENDS(SmallKey, Complex, 8 << 12);
REGISTER_ALL_BACKENDS(SmallKey, LargeValue, 8 << 13);

// Custom composite key: chained seeded hash over integer/string/integer fields.
REGISTER_ALL_BACKENDS(CompositeKey, int, 8 << 11);
REGISTER_ALL_BACKENDS(CompositeKey, Simple, 8 << 11);
REGISTER_ALL_BACKENDS(CompositeKey, Complex, 8 << 10);
REGISTER_ALL_BACKENDS(CompositeKey, LargeValue, 8 << 10);

// Standard containers can safely run the erase workload unconditionally.
REGISTER_STANDARD_ERASES(int, int, 8 << 15);
REGISTER_STANDARD_ERASES(int, Simple, 8 << 15);
REGISTER_STANDARD_ERASES(int, Complex, 8 << 12);
REGISTER_STANDARD_ERASES(int, LargeValue, 8 << 13);
REGISTER_STANDARD_ERASES(std::string, int, 8 << 12);
REGISTER_STANDARD_ERASES(std::string, Simple, 8 << 12);
REGISTER_STANDARD_ERASES(std::string, Complex, 8 << 11);
REGISTER_STANDARD_ERASES(std::string, LargeValue, 8 << 11);
REGISTER_STANDARD_ERASES(SmallKey, int, 8 << 15);
REGISTER_STANDARD_ERASES(SmallKey, Simple, 8 << 15);
REGISTER_STANDARD_ERASES(SmallKey, Complex, 8 << 12);
REGISTER_STANDARD_ERASES(SmallKey, LargeValue, 8 << 13);
REGISTER_STANDARD_ERASES(CompositeKey, int, 8 << 11);
REGISTER_STANDARD_ERASES(CompositeKey, Simple, 8 << 11);
REGISTER_STANDARD_ERASES(CompositeKey, Complex, 8 << 10);
REGISTER_STANDARD_ERASES(CompositeKey, LargeValue, 8 << 10);

// Do not enable until tombstoned objects are also destroyed during rehash and
// Map destruction; otherwise this workload leaks mapped objects and keys.
#define REGISTER_HUH_ERASE(keyType, valueType, maxRange)                              \
    REGISTER_ERASE_BENCHMARK(Backend::HuhMap, keyType, valueType, maxRange)
REGISTER_HUH_ERASE(int, int, 8 << 15);
REGISTER_HUH_ERASE(int, Simple, 8 << 15);
REGISTER_HUH_ERASE(int, Complex, 8 << 12);
REGISTER_HUH_ERASE(int, LargeValue, 8 << 13);
REGISTER_HUH_ERASE(std::string, int, 8 << 12);
REGISTER_HUH_ERASE(std::string, Simple, 8 << 12);
REGISTER_HUH_ERASE(std::string, Complex, 8 << 11);
REGISTER_HUH_ERASE(std::string, LargeValue, 8 << 11);
REGISTER_HUH_ERASE(SmallKey, int, 8 << 15);
REGISTER_HUH_ERASE(SmallKey, Simple, 8 << 15);
REGISTER_HUH_ERASE(SmallKey, Complex, 8 << 12);
REGISTER_HUH_ERASE(SmallKey, LargeValue, 8 << 13);
REGISTER_HUH_ERASE(CompositeKey, int, 8 << 11);
REGISTER_HUH_ERASE(CompositeKey, Simple, 8 << 11);
REGISTER_HUH_ERASE(CompositeKey, Complex, 8 << 10);
REGISTER_HUH_ERASE(CompositeKey, LargeValue, 8 << 10);
#undef REGISTER_HUH_ERASE

// Enable these only after fixing HUH::Map::Iterator. These are the direct
// iteration-speed comparisons against std::map and std::unordered_map.
#define REGISTER_HUH_ITERATION(keyType, valueType, maxRange)                          \
    REGISTER_ITERATION_BENCHMARK(Backend::HuhMap, keyType, valueType, maxRange)

REGISTER_HUH_ITERATION(int, int, 8 << 15);
REGISTER_HUH_ITERATION(int, Simple, 8 << 15);
REGISTER_HUH_ITERATION(int, Complex, 8 << 12);
REGISTER_HUH_ITERATION(int, LargeValue, 8 << 13);
REGISTER_HUH_ITERATION(std::string, int, 8 << 12);
REGISTER_HUH_ITERATION(std::string, Simple, 8 << 12);
REGISTER_HUH_ITERATION(std::string, Complex, 8 << 11);
REGISTER_HUH_ITERATION(std::string, LargeValue, 8 << 11);
REGISTER_HUH_ITERATION(SmallKey, int, 8 << 15);
REGISTER_HUH_ITERATION(SmallKey, Simple, 8 << 15);
REGISTER_HUH_ITERATION(SmallKey, Complex, 8 << 12);
REGISTER_HUH_ITERATION(SmallKey, LargeValue, 8 << 13);
REGISTER_HUH_ITERATION(CompositeKey, int, 8 << 11);
REGISTER_HUH_ITERATION(CompositeKey, Simple, 8 << 11);
REGISTER_HUH_ITERATION(CompositeKey, Complex, 8 << 10);
REGISTER_HUH_ITERATION(CompositeKey, LargeValue, 8 << 10);
#undef REGISTER_HUH_ITERATION

#undef REGISTER_ALL_BACKENDS
#undef REGISTER_ITERATION_BENCHMARK
#undef REGISTER_ERASE_BENCHMARK
#undef REGISTER_STANDARD_ERASES
#undef REGISTER_COMMON_MAP_BENCHMARKS

BENCHMARK_MAIN();
