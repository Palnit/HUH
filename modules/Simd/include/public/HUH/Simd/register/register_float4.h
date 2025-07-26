#pragma once

#include <immintrin.h>

namespace HUH::Simd {
template<>
class Register<float, 4> {
public:
    __m128 data;

    HUH_FORCE_INLINE Register() : data(_mm_setzero_ps()) {}
    HUH_FORCE_INLINE explicit Register(const float v)
        : data(_mm_load1_ps(&v)) {}
    HUH_FORCE_INLINE explicit Register(const float* p)
        : data(_mm_loadu_ps(p)) {}

    template<std::size_t size, std::enable_if_t<size == 4, bool> = true>
    HUH_FORCE_INLINE Register(const float (&t)[size]) noexcept
        : data(_mm_loadu_ps(t)) {}

    explicit Register(float x, float y, float z, float w)
        : data(_mm_setr_ps(x, y, z, w)) {}

    HUH_FORCE_INLINE Register(__m128 v) : data(v) {}

    HUH_FORCE_INLINE operator __m128() const { return data; }

    HUH_FORCE_INLINE void Store(float* ptr) const { _mm_storeu_ps(ptr, data); }

    HUH_FORCE_INLINE Register& operator+=(const Register& rhs) noexcept {
        return *this = (*this + rhs);
    }

    HUH_FORCE_INLINE friend Register operator+(const Register& lhs,
                                               const Register& rhs) noexcept {
        return _mm_add_ps(lhs.data, rhs.data);
    }

    HUH_FORCE_INLINE Register& operator-=(const Register& rhs) noexcept {
        data = _mm_sub_ps(data, rhs.data);
        return *this;
    }

    HUH_FORCE_INLINE friend Register operator-(Register lhs,
                                               const Register& rhs) noexcept {
        lhs.data = _mm_sub_ps(lhs.data, rhs.data);
        return lhs;
    }

    HUH_FORCE_INLINE Register operator-() const noexcept {
        return {_mm_sub_ps(_mm_setzero_ps(), data)};
    }

    HUH_FORCE_INLINE Register& operator*=(const Register& rhs) noexcept {
        data = _mm_mul_ps(data, rhs.data);
        return *this;
    }

    HUH_FORCE_INLINE friend Register operator*(Register lhs,
                                               const Register& rhs) noexcept {
        lhs.data = _mm_mul_ps(lhs.data, rhs.data);
        return lhs;
    }

    HUH_FORCE_INLINE Register& operator*=(const float& rhs) noexcept {
        data = _mm_mul_ps(data, _mm_set1_ps(rhs));
        return *this;
    }

    HUH_FORCE_INLINE friend Register operator*(Register lhs,
                                               const float& rhs) noexcept {
        lhs.data = _mm_mul_ps(lhs.data, _mm_set1_ps(rhs));
        return lhs;
    }

    HUH_FORCE_INLINE Register& operator/=(const Register& rhs) noexcept {
        data = _mm_div_ps(data, rhs.data);
        return *this;
    }

    HUH_FORCE_INLINE friend Register operator/(Register lhs,
                                               const Register& rhs) noexcept {
        lhs.data = _mm_div_ps(lhs.data, rhs.data);
        return lhs;
    }

    HUH_FORCE_INLINE Register& operator/=(const float& rhs) noexcept {
        data = _mm_div_ps(data, _mm_set1_ps(rhs));
        return *this;
    }

    HUH_FORCE_INLINE friend Register operator/(Register lhs,
                                               const float& rhs) noexcept {
        lhs.data = _mm_div_ps(lhs.data, _mm_set1_ps(rhs));
        return lhs;
    }
};
}// namespace HUH::Simd