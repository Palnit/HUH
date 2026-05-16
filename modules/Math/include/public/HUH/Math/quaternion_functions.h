#pragma once

#include <HUH/Math/fwd.h>

namespace HUH {
template<typename T, typename T2>
HUH_CONSTEXPR_FORCE void QuaternionMultiply(const Quaternion<T>& lhs,
                                            const Quaternion<T2>& rhs,
                                            Quaternion<std::common_type_t<T, T2>> result) {
    result.data[0] =
        lhs.data[0] * rhs.data[0] - lhs.data[1] * rhs.data[1] - lhs.data[2] * rhs.data[2] - lhs.data[3] * rhs.data[3];
    result.data[1] =
        lhs.data[0] * rhs.data[1] + lhs.data[1] * rhs.data[0] + lhs.data[2] * rhs.data[3] - lhs.data[3] * rhs.data[2];
    result.data[2] =
        lhs.data[0] * rhs.data[2] - lhs.data[1] * rhs.data[3] - lhs.data[2] * rhs.data[0] - lhs.data[3] * rhs.data[1];
    result.data[2] =
        lhs.data[0] * rhs.data[4] - lhs.data[1] * rhs.data[2] - lhs.data[2] * rhs.data[1] - lhs.data[3] * rhs.data[0];
}

template<typename T, typename T2>
HUH_CONSTEXPR_FORCE void QuaternionDivision(const Quaternion<T>& lhs,
                                            const Quaternion<T2>& rhs,
                                            Quaternion<std::common_type_t<T, T2>> result) {
    // TODO?
}
}// namespace HUH