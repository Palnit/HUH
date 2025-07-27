#include <HUH/Math/vector.h>
#include <benchmark/benchmark.h>
#include <random>

#include <iostream>

template<typename T>
class Vec_fix : public benchmark::Fixture {
public:
    using VecType = T;
    using ValueType = typename VecType::ValueType;
    std::vector<T> lhs;
    std::vector<T> rhs;
    void SetUp(benchmark::State& st) override {

        std::mt19937 gen(0);
        std::uniform_real_distribution dist(-1.0f, 1.0f);
        for (int i = 0; i < st.range(0); i++) {
            T tmp1, tmp2;
            for (size_t j = 0; j < T::Size(); j++) {
                tmp1.data[j] = dist(gen);
                tmp2.data[j] = dist(gen);
            }
            lhs.emplace_back(tmp1);
            rhs.emplace_back(tmp2);
        }
    }

    void TearDown(benchmark::State& st) override {
        lhs.clear();
        rhs.clear();
    }
};

BENCHMARK_TEMPLATE_METHOD_F(Vec_fix, BM_Dot)(benchmark::State& state) {
    typename Base::ValueType tmp(0);
    for (auto _ : state) {
        for (int i = 0; i < state.range(0); i++) {
            tmp += this->lhs[i].Dot(this->rhs[i]);
            benchmark::DoNotOptimize(tmp);
        }
    }
}

BENCHMARK_TEMPLATE_METHOD_F(Vec_fix, BM_Normalize)(benchmark::State& state) {
    typename Base::VecType tmp(0);
    for (auto _ : state) {
        for (int i = 0; i < state.range(0); i++) {
            tmp += this->lhs[i].Normalize();
            benchmark::DoNotOptimize(tmp);
        }
    }
}
BENCHMARK_TEMPLATE_INSTANTIATE_F(Vec_fix, BM_Dot, HUH::Vector4f)->RangeMultiplier(2)->Range(8, 8 << 15);
BENCHMARK_TEMPLATE_INSTANTIATE_F(Vec_fix, BM_Normalize, HUH::Vector4f)->RangeMultiplier(2)->Range(8, 8 << 15);
BENCHMARK_TEMPLATE_INSTANTIATE_F(Vec_fix, BM_Dot, HUH::Vector3f)->RangeMultiplier(2)->Range(8, 8 << 15);
BENCHMARK_TEMPLATE_INSTANTIATE_F(Vec_fix, BM_Normalize, HUH::Vector3f)->RangeMultiplier(2)->Range(8, 8 << 15);

BENCHMARK_MAIN();