#pragma once

#include <HUH/Math/matrix.h>
#include <HUH/Math/quaternion.h>
#include <HUH/Math/vector.h>

namespace HUH::Graphics {
class Transform {
public:
    HUH::Vector3f Position;
    HUH::Quaternionf Rotation;
    HUH::Vector3f Scale;

    HUH::Matrix4x4f ToMatrix() const {
        HUH::Matrix4x4f result = Rotation.ToMatrix();
        result[0][4] = Position[0];
        result[1][4] = Position[1];
        result[2][4] = Position[2];

        result[0][0] *= Scale[0];
        result[1][1] *= Scale[1];
        result[2][2] *= Scale[2];
        return result;
    }

    HUH::Matrix4x4f ToMatrixNoScale() const {
        HUH::Matrix4x4f result = Rotation.ToMatrix();
        result[0][4] = Position[0];
        result[1][4] = Position[1];
        result[2][4] = Position[2];
        return result;
    }
};
}// namespace HUH::Graphics