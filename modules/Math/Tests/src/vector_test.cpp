#include <HUH/Math/vector.h>
#include <gtest/gtest.h>

template<typename T>
class VectorTest : public testing::Test {
protected:
    using VectorType = T;
    using ValueType = typename VectorType::ValueType;
    VectorTest() : rhs(5) {
        for (size_t i = 0; i < T::Size(); ++i) {
            lhs[i] = i + 1;
        }
    }
    T lhs;
    T rhs;
};
using VectorTypes = ::testing::Types<HUH::Vector2f,
                                     HUH::Vector3f,
                                     HUH::Vector4f,
                                     HUH::Vector<float, 5>,
                                     HUH::Vector2d,
                                     HUH::Vector3d,
                                     HUH::Vector4d,
                                     HUH::Vector<double, 5>,
                                     HUH::Vector2i,
                                     HUH::Vector3i,
                                     HUH::Vector4i,
                                     HUH::Vector<int, 5>,
                                     HUH::Vector2ui,
                                     HUH::Vector3ui,
                                     HUH::Vector4ui,
                                     HUH::Vector<unsigned int, 5> >;

TYPED_TEST_SUITE(VectorTest, VectorTypes);

TYPED_TEST(VectorTest, Initializations) {
    for (int i = 0; i < TestFixture::VectorType::Size(); ++i) {
        EXPECT_EQ(this->lhs[i], static_cast<typename TestFixture::ValueType>(i + 1));
        EXPECT_EQ(this->rhs[i], static_cast<typename TestFixture::ValueType>(5));
    }
}

TYPED_TEST(VectorTest, Addition) {
    const auto result = this->lhs + this->rhs;
    EXPECT_EQ(result.Size(), TestFixture::VectorType::Size());
    for (int i = 0; i < TestFixture::VectorType::Size(); ++i) {
        EXPECT_EQ(result[i], static_cast<typename TestFixture::ValueType>((i + 1) + 5));
    }
}
TYPED_TEST(VectorTest, Subtraction) {
    const auto result = this->lhs - this->rhs;
    EXPECT_EQ(result.Size(), TestFixture::VectorType::Size());
    for (int i = 0; i < TestFixture::VectorType::Size(); ++i) {
        EXPECT_EQ(result[i], static_cast<typename TestFixture::ValueType>((i + 1) - 5));
    }
}
