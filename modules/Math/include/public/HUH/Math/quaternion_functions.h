#pragma once

#include <HUH/Math/fwd.h>
#include <HUH/definitions.h>

namespace HUH {
template<typename T, typename T2>
HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE void QuaternionMultiply(const Quaternion<T>& lhs,
                                                                const Quaternion<T2>& rhs,
                                                                Quaternion<std::common_type_t<T, T2>>& result) {
    result.data[0] =
        lhs.data[0] * rhs.data[0] - lhs.data[1] * rhs.data[1] - lhs.data[2] * rhs.data[2] - lhs.data[3] * rhs.data[3];
    result.data[1] =
        lhs.data[0] * rhs.data[1] + lhs.data[1] * rhs.data[0] + lhs.data[2] * rhs.data[3] - lhs.data[3] * rhs.data[2];
    result.data[2] =
        lhs.data[0] * rhs.data[2] - lhs.data[1] * rhs.data[3] + lhs.data[2] * rhs.data[0] + lhs.data[3] * rhs.data[1];
    result.data[3] =
        lhs.data[0] * rhs.data[3] + lhs.data[1] * rhs.data[2] - lhs.data[2] * rhs.data[1] + lhs.data[3] * rhs.data[0];
}

template<typename T, typename T2>
HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE void QuaternionDivision(const Quaternion<T>& lhs,
                                                                const Quaternion<T2>& rhs,
                                                                Quaternion<std::common_type_t<T, T2>>& result) {
    // TODO?
}

// Algorithm from:
// https://gamedev.stackexchange.com/questions/28395/rotating-vector3-by-a-quaternion
template<typename T>
HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE void RotateVector(const Quaternion<T>& quat,
                                                          const Vector3<T>& vec,
                                                          Vector3<T>& result) {
    HUH::Vector3<T> u(quat.data[1], quat.data[2], quat.data[3]);

    result =
        u * 2.0f * u.Dot(vec) + vec * (quat.data[0] * quat.data[0] - u.Dot(u)) + u.Cross(vec) * 2.0f * quat.data[0];
}

template<typename T>
HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE void Normalize(HUH::Quaternion<T>& quat) noexcept {
    T tmp = quat.data[0] * quat.data[0] + quat.data[1] * quat.data[1] + quat.data[2] * quat.data[2]
        + quat.data[3] * quat.data[3];
    tmp = std::sqrt(tmp);
    quat /= tmp;
}

template<typename T>
HUH_HOST HUH_DEVICE HUH_CONSTEXPR_FORCE Matrix4x4<T> ToMatrix(HUH::Quaternion<T> quat) noexcept {
    quat.Normalize();
    return {{1 - 2 * (quat[2] * quat[2] + quat[3] * quat[3]), 2 * (quat[1] * quat[2] - quat[3] * quat[0]),
             2 * (quat[1] * quat[3] + quat[2] * quat[0]), 0},
            {2 * (quat[1] * quat[2] + quat[3] * quat[0]), 1 - 2 * (quat[1] * quat[1] + quat[3] * quat[3]),
             2 * (quat[2] * quat[3] - quat[1] * quat[0]), 0},
            {2 * (quat[1] * quat[3] - quat[2] * quat[0]), 2 * (quat[2] * quat[3] + quat[1] * quat[0]),
             1 - 2 * (quat[1] * quat[1] + quat[2] * quat[2]), 0},
            {0, 0, 0, 1}};
}

}// namespace HUH