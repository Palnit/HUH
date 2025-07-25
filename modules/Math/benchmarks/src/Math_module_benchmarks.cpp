#include <HUH/Math/vector.h>
#include <benchmark/benchmark.h>

#include <iostream>

static void BM_Vec4f_dot(benchmark::State& state) {
    std::vector<HUH::Vector4f> lhs(10000);
    std::vector<HUH::Vector4f> rhs(10000);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution dist(0.0f, 1.0f);
    for (int i = 0; i < 10000; i++) {
        lhs.emplace_back(dist(gen));
        rhs.emplace_back(dist(gen));
    }
    float tmp(0);
    for (auto _ : state) {
        for (int i = 0; i < 10000; i++) {
            tmp += lhs[i].Dot(rhs[i]);
            benchmark::DoNotOptimize(tmp);
        }
    }
}

static void BM_Vec4f_normalize(benchmark::State& state) {
    std::vector<HUH::Vector4f> lhs(10000);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution dist(0.0f, 1.0f);
    for (int i = 0; i < 10000; i++) { lhs.emplace_back(dist(gen)); }

    HUH::Vector4f tmp(0);
    for (auto _ : state) {
        for (int i = 0; i < 10000; i++) {
            tmp += lhs[i].Normalize();
            benchmark::DoNotOptimize(tmp);
        }
    }
}

BENCHMARK(BM_Vec4f_dot);
BENCHMARK(BM_Vec4f_normalize);

BENCHMARK_MAIN();