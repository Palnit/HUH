#include <HUH/Math/vector.h>
#include <benchmark/benchmark.h>
#include <random>

#include <iostream>

class Vec4f : public benchmark::Fixture {
public:
    std::vector<HUH::Vector4f> lhs;
    std::vector<HUH::Vector4f> rhs;
    void SetUp(benchmark::State& st) override {

        std::mt19937 gen(0);
        std::uniform_real_distribution dist(-1.0f, 1.0f);
        for (int i = 0; i < st.range(0); i++) {
            lhs.emplace_back(dist(gen));
            rhs.emplace_back(dist(gen));
        }
    }

    void TearDown(benchmark::State& st) override {
        lhs.clear();
        rhs.clear();
    }
};

BENCHMARK_DEFINE_F(Vec4f, BM_Dot)(benchmark::State& state) {
    float tmp(0);
    for (auto _ : state) {
        for (int i = 0; i < state.range(0); i++) {
            tmp += lhs[i].Dot(rhs[i]);
            benchmark::DoNotOptimize(tmp);
        }
    }
}

BENCHMARK_DEFINE_F(Vec4f, BM_Normalize)(benchmark::State& state) {
    HUH::Vector4f tmp(0);
    for (auto _ : state) {
        for (int i = 0; i < state.range(0); i++) {
            tmp += lhs[i].Normalize();
            benchmark::DoNotOptimize(tmp);
        }
    }
}
BENCHMARK_REGISTER_F(Vec4f, BM_Dot)->RangeMultiplier(2)->Range(8, 8 << 15);
BENCHMARK_REGISTER_F(Vec4f, BM_Normalize)
    ->RangeMultiplier(2)
    ->Range(8, 8 << 15);

BENCHMARK_MAIN();