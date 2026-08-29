#include <HUH/Types/array.h>
#include <benchmark/benchmark.h>

struct Simple {
    int a;
    Simple& operator+=(const Simple& other) {
        a += other.a;
        return *this;
    }
};

class Complex {
public:
    Complex() : a(0), b(nullptr) {}
    explicit Complex(int a) : a(a), b(new Simple(1)) {}

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

template<typename T>
static void BM_push_back_std(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<T> v;
        for (size_t i = 0; i < state.range(0); i++) {
            v.emplace_back(i);
        }
    }
}

template<typename T>
static void BM_push_back_huh(benchmark::State& state) {
    for (auto _ : state) {
        HUH::Array<T> v;
        for (size_t i = 0; i < state.range(0); i++) {
            v.Emplace(i);
        }
    }
}

template<typename T>
static void BM_reserve_std(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<T> v;
        v.reserve(state.range(0));
        for (size_t i = 0; i < state.range(0); i++) {
            v.emplace_back(i);
        }
    }
}

template<typename T>
static void BM_reserve_huh(benchmark::State& state) {
    for (auto _ : state) {
        HUH::Array<T> v;
        v.Reserve(state.range(0));
        for (size_t i = 0; i < state.range(0); i++) {
            v.Emplace(i);
        }
    }
}

template<typename T>
static void BM_iteration_std(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        std::vector<T> v;
        v.reserve(state.range(0));
        for (size_t i = 0; i < state.range(0); i++) {
            v.emplace_back(i);
        }
        T test;
        state.ResumeTiming();
        for (auto& element : v) {
            benchmark::DoNotOptimize(test += element);
        }
    }
}

template<typename T>
static void BM_iteration_huh(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        HUH::Array<T> v;
        v.Reserve(state.range(0));
        for (size_t i = 0; i < state.range(0); i++) {
            v.Emplace(i);
        }
        T test;
        state.ResumeTiming();
        for (auto& element : v) {
            benchmark::DoNotOptimize(test += element);
        }
    }
}

BENCHMARK(BM_push_back_std<int>)->RangeMultiplier(2)->Range(8, 8 << 15);
BENCHMARK(BM_push_back_huh<int>)->RangeMultiplier(2)->Range(8, 8 << 15);

BENCHMARK(BM_reserve_std<int>)->RangeMultiplier(2)->Range(8, 8 << 15);
BENCHMARK(BM_reserve_huh<int>)->RangeMultiplier(2)->Range(8, 8 << 15);

BENCHMARK(BM_iteration_std<int>)->RangeMultiplier(2)->Range(8, 8 << 15);
BENCHMARK(BM_iteration_huh<int>)->RangeMultiplier(2)->Range(8, 8 << 15);

BENCHMARK(BM_push_back_std<Simple>)->RangeMultiplier(2)->Range(8, 8 << 15);
BENCHMARK(BM_push_back_huh<Simple>)->RangeMultiplier(2)->Range(8, 8 << 15);

BENCHMARK(BM_reserve_std<Simple>)->RangeMultiplier(2)->Range(8, 8 << 15);
BENCHMARK(BM_reserve_huh<Simple>)->RangeMultiplier(2)->Range(8, 8 << 15);

BENCHMARK(BM_iteration_std<Simple>)->RangeMultiplier(2)->Range(8, 8 << 15);
BENCHMARK(BM_iteration_huh<Simple>)->RangeMultiplier(2)->Range(8, 8 << 15);

BENCHMARK(BM_push_back_std<Complex>)->RangeMultiplier(2)->Range(8, 8 << 15);
BENCHMARK(BM_push_back_huh<Complex>)->RangeMultiplier(2)->Range(8, 8 << 15);

BENCHMARK(BM_reserve_std<Complex>)->RangeMultiplier(2)->Range(8, 8 << 15);
BENCHMARK(BM_reserve_huh<Complex>)->RangeMultiplier(2)->Range(8, 8 << 15);

BENCHMARK(BM_iteration_std<Complex>)->RangeMultiplier(2)->Range(8, 8 << 15);
BENCHMARK(BM_iteration_huh<Complex>)->RangeMultiplier(2)->Range(8, 8 << 15);

BENCHMARK_MAIN();
