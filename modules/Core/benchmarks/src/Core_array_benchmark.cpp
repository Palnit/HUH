#include <HUH/Types/array.h>
#include <benchmark/benchmark.h>

static void BM_push_back_std(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<size_t> v;
        for (size_t i = 0; i < state.range(0); i++) {
            v.push_back(i);
        }
    }
}

static void BM_push_back_huh(benchmark::State& state) {
    for (auto _ : state) {
        HUH::Array<size_t> v;
        for (size_t i = 0; i < state.range(0); i++) {
            v.Emplace(i);
        }
    }
}

BENCHMARK(BM_push_back_std)->RangeMultiplier(2)->Range(8, 8 << 15);
BENCHMARK(BM_push_back_huh)->RangeMultiplier(2)->Range(8, 8 << 15);

BENCHMARK_MAIN();
