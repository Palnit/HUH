#include <HUH/Math/matrix.h>
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

template<typename T>
class Matrix_fix : public benchmark::Fixture {
public:
    using MatrixType = T;
    using ValueType = typename MatrixType::ValueType;
    std::vector<T> lhs;
    std::vector<T> rhs;

    void SetUp(benchmark::State& st) override {

        std::mt19937 gen(0);
        std::uniform_real_distribution dist(-1.0f, 1.0f);
        for (int i = 0; i < st.range(0); i++) {
            T tmp1, tmp2;
            for (size_t j = 0; j < T::RowSize(); j++) {
                for (size_t k = 0; k < T::ColumnSize(); k++) {
                    tmp1.data[j][k] = dist(gen);
                    tmp2.data[j][k] = dist(gen);
                }
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
    for (auto _ : state) {
        for (int i = 0; i < state.range(0); i++) {
            benchmark::DoNotOptimize(this->lhs[i].Dot(this->rhs[i]));
        }
    }
}

BENCHMARK_TEMPLATE_METHOD_F(Vec_fix, BM_Normalize)(benchmark::State& state) {
    for (auto _ : state) {
        for (int i = 0; i < state.range(0); i++) {
            benchmark::DoNotOptimize(this->lhs[i].Normalize());
        }
    }
}

BENCHMARK_TEMPLATE_METHOD_F(Matrix_fix, BM_Multiplication)(benchmark::State& state) {
    for (auto _ : state) {
        for (int i = 0; i < state.range(0); i++) {
            benchmark::DoNotOptimize(this->lhs[i] * this->rhs[i]);
        }
    }
}

BENCHMARK_TEMPLATE_INSTANTIATE_F(Vec_fix, BM_Dot, HUH::Vector4f)->RangeMultiplier(2)->Range(8, 8 << 15);
BENCHMARK_TEMPLATE_INSTANTIATE_F(Vec_fix, BM_Normalize, HUH::Vector4f)->RangeMultiplier(2)->Range(8, 8 << 15);
BENCHMARK_TEMPLATE_INSTANTIATE_F(Vec_fix, BM_Dot, HUH::Vector3f)->RangeMultiplier(2)->Range(8, 8 << 15);
BENCHMARK_TEMPLATE_INSTANTIATE_F(Vec_fix, BM_Normalize, HUH::Vector3f)->RangeMultiplier(2)->Range(8, 8 << 15);
BENCHMARK_TEMPLATE_INSTANTIATE_F(Matrix_fix, BM_Multiplication, HUH::Matrix4x4f)->RangeMultiplier(2)->Range(8, 8 << 15);

BENCHMARK_MAIN();