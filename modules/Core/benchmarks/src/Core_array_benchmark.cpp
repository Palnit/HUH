#include <HUH/Types/array.h>

#include <benchmark/benchmark.h>

#include <array>
#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <utility>
#include <vector>

///
/// DISCLAIMER CHATGPT GENERATED BENCHMARKS I wrote the bases for this but used ChatGPT to generate the rest.
///

struct Simple {
    int a = 0;

    Simple& operator+=(const Simple& other) {
        a += other.a;
        return *this;
    }
};

class Complex {
public:
    Complex() : a(0), b(nullptr) {}
    explicit Complex(int value) : a(value), b(new Simple{1}) {}

    Complex& operator+=(const Complex& other) {
        a += other.a;
        return *this;
    }

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

template<typename T, bool UseHuhArray>
using Container = std::conditional_t<UseHuhArray, HUH::Array<T>, std::vector<T>>;

template<typename T, bool UseHuhArray>
static void Reserve(Container<T, UseHuhArray>& container, std::size_t count) {
    if constexpr (UseHuhArray) {
        container.Reserve(count);
    } else {
        container.reserve(count);
    }
}

template<typename T, bool UseHuhArray>
static void Append(Container<T, UseHuhArray>& container, int value) {
    if constexpr (UseHuhArray) {
        container.Emplace(value);
    } else {
        container.emplace_back(value);
    }
}

template<typename T, bool UseHuhArray>
static void Clear(Container<T, UseHuhArray>& container) {
    if constexpr (UseHuhArray) {
        container.Clear();
    } else {
        container.clear();
    }
}

template<typename T, bool UseHuhArray>
static void BM_PushBack(benchmark::State& state) {
    const auto count = static_cast<std::size_t>(state.range(0));
    for (auto _ : state) {
        Container<T, UseHuhArray> container;
        for (std::size_t i = 0; i < count; ++i) {
            Append<T, UseHuhArray>(container, static_cast<int>(i));
        }
        benchmark::DoNotOptimize(container.data());
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(state.iterations() * static_cast<std::int64_t>(count));
}

// Measures the common known-size construction path, including Reserve itself.
template<typename T, bool UseHuhArray>
static void BM_ReserveThenPushBack(benchmark::State& state) {
    const auto count = static_cast<std::size_t>(state.range(0));
    for (auto _ : state) {
        Container<T, UseHuhArray> container;
        Reserve<T, UseHuhArray>(container, count);
        for (std::size_t i = 0; i < count; ++i) {
            Append<T, UseHuhArray>(container, static_cast<int>(i));
        }
        benchmark::DoNotOptimize(container.data());
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(state.iterations() * static_cast<std::int64_t>(count));
}

// Measures reuse of already-allocated storage: clear and refill on each iteration.
template<typename T, bool UseHuhArray>
static void BM_ClearAndRefill(benchmark::State& state) {
    const auto count = static_cast<std::size_t>(state.range(0));
    Container<T, UseHuhArray> container;
    Reserve<T, UseHuhArray>(container, count);

    for (auto _ : state) {
        Clear<T, UseHuhArray>(container);
        for (std::size_t i = 0; i < count; ++i) {
            Append<T, UseHuhArray>(container, static_cast<int>(i));
        }
        benchmark::DoNotOptimize(container.data());
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(state.iterations() * static_cast<std::int64_t>(count));
}

// Setup is outside the timed loop so this isolates sequential traversal.
template<typename T, bool UseHuhArray>
static void BM_Iteration(benchmark::State& state) {
    const auto count = static_cast<std::size_t>(state.range(0));
    Container<T, UseHuhArray> container;
    Reserve<T, UseHuhArray>(container, count);
    for (std::size_t i = 0; i < count; ++i) {
        Append<T, UseHuhArray>(container, static_cast<int>(i));
    }

    for (auto _ : state) {
        T total{};
        for (const auto& value : container) {
            total += value;
        }
        benchmark::DoNotOptimize(total);
    }
    state.SetItemsProcessed(state.iterations() * static_cast<std::int64_t>(count));
}

template<typename T, bool UseHuhArray>
static void BM_IndexedAccess(benchmark::State& state) {
    const auto count = static_cast<std::size_t>(state.range(0));
    Container<T, UseHuhArray> container;
    Reserve<T, UseHuhArray>(container, count);
    for (std::size_t i = 0; i < count; ++i) {
        Append<T, UseHuhArray>(container, static_cast<int>(i));
    }

    for (auto _ : state) {
        T total{};
        for (std::size_t i = 0; i < count; ++i) {
            total += container[i];
        }
        benchmark::DoNotOptimize(total);
    }
    state.SetItemsProcessed(state.iterations() * static_cast<std::int64_t>(count));
}

template<bool UseHuhArray>
static void BM_CopyConstructionInt(benchmark::State& state) {
    const auto count = static_cast<std::size_t>(state.range(0));
    Container<int, UseHuhArray> source;
    Reserve<int, UseHuhArray>(source, count);
    for (std::size_t i = 0; i < count; ++i) {
        Append<int, UseHuhArray>(source, static_cast<int>(i));
    }

    for (auto _ : state) {
        Container<int, UseHuhArray> copy(source);
        benchmark::DoNotOptimize(copy.data());
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(state.iterations() * static_cast<std::int64_t>(count));
}

#define HUH_REGISTER_TYPE_PAIR(functionName, typeName, maxRange)                  \
    BENCHMARK_TEMPLATE(functionName, typeName, false)                             \
        ->RangeMultiplier(2)->Range(8, maxRange);                                 \
    BENCHMARK_TEMPLATE(functionName, typeName, true)                              \
        ->RangeMultiplier(2)->Range(8, maxRange)

#define HUH_REGISTER_BOOL_PAIR(functionName, maxRange)                            \
    BENCHMARK_TEMPLATE(functionName, false)->RangeMultiplier(2)->Range(8, maxRange); \
    BENCHMARK_TEMPLATE(functionName, true)->RangeMultiplier(2)->Range(8, maxRange)

HUH_REGISTER_TYPE_PAIR(BM_PushBack, int, 8 << 15);
HUH_REGISTER_TYPE_PAIR(BM_ReserveThenPushBack, int, 8 << 15);
HUH_REGISTER_TYPE_PAIR(BM_ClearAndRefill, int, 8 << 15);
HUH_REGISTER_TYPE_PAIR(BM_Iteration, int, 8 << 15);
HUH_REGISTER_TYPE_PAIR(BM_IndexedAccess, int, 8 << 15);

HUH_REGISTER_TYPE_PAIR(BM_PushBack, Simple, 8 << 15);
HUH_REGISTER_TYPE_PAIR(BM_ReserveThenPushBack, Simple, 8 << 15);
HUH_REGISTER_TYPE_PAIR(BM_ClearAndRefill, Simple, 8 << 15);
HUH_REGISTER_TYPE_PAIR(BM_Iteration, Simple, 8 << 15);
HUH_REGISTER_TYPE_PAIR(BM_IndexedAccess, Simple, 8 << 15);

// Complex deliberately allocates a payload for every live element and is move-only.
HUH_REGISTER_TYPE_PAIR(BM_PushBack, Complex, 8 << 12);
HUH_REGISTER_TYPE_PAIR(BM_ReserveThenPushBack, Complex, 8 << 12);
HUH_REGISTER_TYPE_PAIR(BM_ClearAndRefill, Complex, 8 << 12);
HUH_REGISTER_TYPE_PAIR(BM_Iteration, Complex, 8 << 12);
HUH_REGISTER_TYPE_PAIR(BM_IndexedAccess, Complex, 8 << 12);

// A larger trivially movable value highlights bandwidth/cache effects.
HUH_REGISTER_TYPE_PAIR(BM_PushBack, LargeValue, 8 << 13);
HUH_REGISTER_TYPE_PAIR(BM_ReserveThenPushBack, LargeValue, 8 << 13);
HUH_REGISTER_TYPE_PAIR(BM_Iteration, LargeValue, 8 << 13);
HUH_REGISTER_TYPE_PAIR(BM_IndexedAccess, LargeValue, 8 << 13);

HUH_REGISTER_BOOL_PAIR(BM_CopyConstructionInt, 8 << 15);

#undef HUH_REGISTER_TYPE_PAIR
#undef HUH_REGISTER_BOOL_PAIR

BENCHMARK_MAIN();
