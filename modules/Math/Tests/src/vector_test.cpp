#include <HUH/Math/vector.h>
#include <gtest/gtest.h>

class VectorTest : public testing::Test {
protected:
    VectorTest()
        : Vec5i(),
          Vec5f({5.f}),
          Vec5d(5.0),
          Vec4i(1, 2, 3, 4),
          Vec4f(4),
          Vec4d({1, 2, 3, 4}),
          Vec3i(1, 2, 3),
          Vec3f(3),
          Vec3d({1, 2, 3}),
          Vec2i(1, 2),
          Vec2f(2),
          Vec2d({1, 2})

    {}

    HUH::Vector<int, 5> Vec5i;
    HUH::Vector<float, 5> Vec5f;
    HUH::Vector<double, 5> Vec5d;

    HUH::Vector4i Vec4i;
    HUH::Vector4f Vec4f;
    HUH::Vector4d Vec4d;

    HUH::Vector3i Vec3i;
    HUH::Vector3f Vec3f;
    HUH::Vector3d Vec3d;

    HUH::Vector2i Vec2i;
    HUH::Vector2f Vec2f;
    HUH::Vector2d Vec2d;

    int Vec5iEq[5] = {0, 0, 0, 0, 0};
    float Vec5fEq[5] = {5.f, 5.f, 5.f, 5.f, 5.f};
    double Vec5dEq[5] = {5.0, 5.0, 5.0, 5.0, 5.0};

    int Vec4iEq[4] = {1, 2, 3, 4};
    float Vec4fEq[4] = {4, 4, 4, 4};
    double Vec4dEq[4] = {1, 2, 3, 4};

    int Vec3iEq[3] = {1, 2, 3};
    float Vec3fEq[3] = {3, 3, 3};
    double Vec3dEq[3] = {1, 2, 3};

    int Vec2iEq[2] = {1, 2};
    float Vec2fEq[2] = {2, 2};
    double Vec2dEq[2] = {1, 2};
};

TEST_F(VectorTest, Initializations) {
    for (int i = 0; i < 5; i++) {
        EXPECT_EQ(Vec5i[i], Vec5iEq[i]);
        EXPECT_EQ(Vec5f[i], Vec5fEq[i]);
        EXPECT_EQ(Vec5d[i], Vec5dEq[i]);
    }

    for (int i = 0; i < 4; i++) {
        EXPECT_EQ(Vec4i[i], Vec4iEq[i]);
        EXPECT_EQ(Vec4f[i], Vec4fEq[i]);
        EXPECT_EQ(Vec4d[i], Vec4dEq[i]);
    }

    for (int i = 0; i < 3; i++) {
        EXPECT_EQ(Vec3i[i], Vec3iEq[i]);
        EXPECT_EQ(Vec3f[i], Vec3fEq[i]);
        EXPECT_EQ(Vec3d[i], Vec3dEq[i]);
    }

    for (int i = 0; i < 2; i++) {
        EXPECT_EQ(Vec2i[i], Vec2iEq[i]);
        EXPECT_EQ(Vec2f[i], Vec2fEq[i]);
        EXPECT_EQ(Vec2d[i], Vec2dEq[i]);
    }
}