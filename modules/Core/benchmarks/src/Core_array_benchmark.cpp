#include <HUH/Types/array.h>

#include <benchmark/benchmark.h>

#include <array>
#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <utility>
#include <vector>

///
/// DISCLAIMER: CHATGPT-GENERATED BENCHMARKS. I wrote the bases for this but
/// used ChatGPT to generate the rest.
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

template<template<typename...> typename ContainerType, typename T>
static void Reserve(ContainerType<T>& container, std::size_t count) {
    if constexpr (std::is_same_v<ContainerType<T>, HUH::Array<T>>) {
        container.Reserve(count);
    } else {
        container.reserve(count);
    }
}

template<template<typename...> typename ContainerType, typename T>
static void Append(ContainerType<T>& container, int value) {
    if constexpr (std::is_same_v<ContainerType<T>, HUH::Array<T>>) {
        container.Emplace(value);
    } else {
        container.emplace_back(value);
    }
}

template<template<typename...> typename ContainerType, typename T>
static void Clear(ContainerType<T>& container) {
    if constexpr (std::is_same_v<ContainerType<T>, HUH::Array<T>>) {
        container.Clear();
    } else {
        container.clear();
    }
}

template<template<typename...> typename ContainerType, typename T>
static void BM_PushBack(benchmark::State& state) {
    const auto count = static_cast<std::size_t>(state.range(0));
    for (auto _ : state) {
        ContainerType<T> container;
        for (std::size_t i = 0; i < count; ++i) {
            Append<ContainerType, T>(container, static_cast<int>(i));
        }
        benchmark::DoNotOptimize(container.data());
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(state.iterations() * static_cast<std::int64_t>(count));
}

// Measures the common known-size construction path, including Reserve itself.
template<template<typename...> typename ContainerType, typename T>
static void BM_ReserveThenPushBack(benchmark::State& state) {
    const auto count = static_cast<std::size_t>(state.range(0));
    for (auto _ : state) {
        ContainerType<T> container;
        Reserve<ContainerType, T>(container, count);
        for (std::size_t i = 0; i < count; ++i) {
            Append<ContainerType, T>(container, static_cast<int>(i));
        }
        benchmark::DoNotOptimize(container.data());
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(state.iterations() * static_cast<std::int64_t>(count));
}

// Measures reuse of already-allocated storage: clear and refill on each iteration.
template<template<typename...> typename ContainerType, typename T>
static void BM_ClearAndRefill(benchmark::State& state) {
    const auto count = static_cast<std::size_t>(state.range(0));
    ContainerType<T> container;
    Reserve<ContainerType, T>(container, count);

    for (auto _ : state) {
        Clear<ContainerType, T>(container);
        for (std::size_t i = 0; i < count; ++i) {
            Append<ContainerType, T>(container, static_cast<int>(i));
        }
        benchmark::DoNotOptimize(container.data());
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(state.iterations() * static_cast<std::int64_t>(count));
}

// Setup is outside the timed loop so this isolates sequential traversal.
template<template<typename...> typename ContainerType, typename T>
static void BM_Iteration(benchmark::State& state) {
    const auto count = static_cast<std::size_t>(state.range(0));
    ContainerType<T> container;
    Reserve<ContainerType, T>(container, count);
    for (std::size_t i = 0; i < count; ++i) {
        Append<ContainerType, T>(container, static_cast<int>(i));
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

template<template<typename...> typename ContainerType, typename T>
static void BM_IndexedAccess(benchmark::State& state) {
    const auto count = static_cast<std::size_t>(state.range(0));
    ContainerType<T> container;
    Reserve<ContainerType, T>(container, count);
    for (std::size_t i = 0; i < count; ++i) {
        Append<ContainerType, T>(container, static_cast<int>(i));
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

template<template<typename...> typename ContainerType, typename T>
static void BM_CopyConstructionInt(benchmark::State& state) {
    const auto count = static_cast<std::size_t>(state.range(0));
    ContainerType<T> source;
    Reserve<ContainerType, T>(source, count);
    for (std::size_t i = 0; i < count; ++i) {
        Append<ContainerType, T>(source, static_cast<int>(i));
    }

    for (auto _ : state) {
        ContainerType<T> copy(source);
        benchmark::DoNotOptimize(copy.data());
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(state.iterations() * static_cast<std::int64_t>(count));
}

#define HUH_REGISTER_TYPE_PAIR(functionName, containerType, valueType, maxRange) \
    BENCHMARK_TEMPLATE(functionName, containerType, valueType)                    \
        ->RangeMultiplier(2)->Range(8, maxRange)

HUH_REGISTER_TYPE_PAIR(BM_PushBack, std::vector, int, 8 << 15);
HUH_REGISTER_TYPE_PAIR(BM_ReserveThenPushBack, std::vector, int, 8 << 15);
HUH_REGISTER_TYPE_PAIR(BM_ClearAndRefill, std::vector, int, 8 << 15);
HUH_REGISTER_TYPE_PAIR(BM_Iteration, std::vector, int, 8 << 15);
HUH_REGISTER_TYPE_PAIR(BM_IndexedAccess, std::vector, int, 8 << 15);

HUH_REGISTER_TYPE_PAIR(BM_PushBack, HUH::Array, int, 8 << 15);
HUH_REGISTER_TYPE_PAIR(BM_ReserveThenPushBack, HUH::Array, int, 8 << 15);
HUH_REGISTER_TYPE_PAIR(BM_ClearAndRefill, HUH::Array, int, 8 << 15);
HUH_REGISTER_TYPE_PAIR(BM_Iteration, HUH::Array, int, 8 << 15);
HUH_REGISTER_TYPE_PAIR(BM_IndexedAccess, HUH::Array, int, 8 << 15);

HUH_REGISTER_TYPE_PAIR(BM_PushBack, std::vector, Simple, 8 << 15);
HUH_REGISTER_TYPE_PAIR(BM_ReserveThenPushBack, std::vector, Simple, 8 << 15);
HUH_REGISTER_TYPE_PAIR(BM_ClearAndRefill, std::vector, Simple, 8 << 15);
HUH_REGISTER_TYPE_PAIR(BM_Iteration, std::vector, Simple, 8 << 15);
HUH_REGISTER_TYPE_PAIR(BM_IndexedAccess, std::vector, Simple, 8 << 15);

HUH_REGISTER_TYPE_PAIR(BM_PushBack, HUH::Array, Simple, 8 << 15);
HUH_REGISTER_TYPE_PAIR(BM_ReserveThenPushBack, HUH::Array, Simple, 8 << 15);
HUH_REGISTER_TYPE_PAIR(BM_ClearAndRefill, HUH::Array, Simple, 8 << 15);
HUH_REGISTER_TYPE_PAIR(BM_Iteration, HUH::Array, Simple, 8 << 15);
HUH_REGISTER_TYPE_PAIR(BM_IndexedAccess, HUH::Array, Simple, 8 << 15);

// Complex deliberately allocates a payload for every live element and is move-only.
HUH_REGISTER_TYPE_PAIR(BM_PushBack, std::vector, Complex, 8 << 12);
HUH_REGISTER_TYPE_PAIR(BM_ReserveThenPushBack, std::vector, Complex, 8 << 12);
HUH_REGISTER_TYPE_PAIR(BM_ClearAndRefill, std::vector, Complex, 8 << 12);
HUH_REGISTER_TYPE_PAIR(BM_Iteration, std::vector, Complex, 8 << 12);
HUH_REGISTER_TYPE_PAIR(BM_IndexedAccess, std::vector, Complex, 8 << 12);

HUH_REGISTER_TYPE_PAIR(BM_PushBack, HUH::Array, Complex, 8 << 12);
HUH_REGISTER_TYPE_PAIR(BM_ReserveThenPushBack, HUH::Array, Complex, 8 << 12);
HUH_REGISTER_TYPE_PAIR(BM_ClearAndRefill, HUH::Array, Complex, 8 << 12);
HUH_REGISTER_TYPE_PAIR(BM_Iteration, HUH::Array, Complex, 8 << 12);
HUH_REGISTER_TYPE_PAIR(BM_IndexedAccess, HUH::Array, Complex, 8 << 12);

// A larger trivially movable value highlights bandwidth/cache effects.
HUH_REGISTER_TYPE_PAIR(BM_PushBack, std::vector, LargeValue, 8 << 13);
HUH_REGISTER_TYPE_PAIR(BM_ReserveThenPushBack, std::vector, LargeValue, 8 << 13);
HUH_REGISTER_TYPE_PAIR(BM_Iteration, std::vector, LargeValue, 8 << 13);
HUH_REGISTER_TYPE_PAIR(BM_IndexedAccess, std::vector, LargeValue, 8 << 13);

HUH_REGISTER_TYPE_PAIR(BM_PushBack, HUH::Array, LargeValue, 8 << 13);
HUH_REGISTER_TYPE_PAIR(BM_ReserveThenPushBack, HUH::Array, LargeValue, 8 << 13);
HUH_REGISTER_TYPE_PAIR(BM_Iteration, HUH::Array, LargeValue, 8 << 13);
HUH_REGISTER_TYPE_PAIR(BM_IndexedAccess, HUH::Array, LargeValue, 8 << 13);

HUH_REGISTER_TYPE_PAIR(BM_CopyConstructionInt, std::vector, int, 8 << 15);
HUH_REGISTER_TYPE_PAIR(BM_CopyConstructionInt, HUH::Array, int, 8 << 15);

#undef HUH_REGISTER_TYPE_PAIR

BENCHMARK_MAIN();
