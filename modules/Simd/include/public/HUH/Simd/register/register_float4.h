#pragma once

#include <xmmintrin.h>

namespace HUH::Simd {
template<>
class Register<float, 4> {
public:
    __m128 data;

    inline Register() : data(_mm_setzero_ps()) {}
    inline explicit Register(const float v) : data(_mm_load1_ps(&v)) {}
    inline explicit Register(const float* p) : data(_mm_loadu_ps(p)) {}

    template<std::size_t size, std::enable_if_t<size == 4, bool> = true>
    inline Register(const float (&t)[size]) noexcept : data(_mm_loadu_ps(t)) {}

    explicit Register(float x, float y, float z, float w)
        : data(_mm_setr_ps(x, y, z, w)) {}

    inline Register(__m128 v) : data(v) {}

    inline operator __m128() const { return data; }

    inline void Store(float* ptr) const { _mm_store_ps(ptr, data); }

    inline Register& operator+=(const Register& rhs) noexcept {
        return *this = (*this + rhs);
    }

    inline friend Register operator+(const Register& lhs,
                                     const Register& rhs) noexcept {
        return _mm_add_ps(lhs.data, rhs.data);
    }

    Register& operator-=(const Register& rhs) noexcept {
        data = _mm_sub_ps(data, rhs.data);
        return *this;
    }

    friend Register operator-(Register lhs, const Register& rhs) noexcept {
        lhs.data = _mm_sub_ps(lhs.data, rhs.data);
        return lhs;
    }

    Register operator-() const noexcept {
        return {_mm_sub_ps(_mm_setzero_ps(), data)};
    }

    inline Register& operator*=(const Register& rhs) noexcept {
        data = _mm_mul_ps(data, rhs.data);
        return *this;
    }

    inline friend Register operator*(Register lhs,
                                     const Register& rhs) noexcept {
        lhs.data = _mm_mul_ps(lhs.data, rhs.data);
        return lhs;
    }

    Register& operator*=(const float& rhs) noexcept {
        data = _mm_mul_ps(data, _mm_set1_ps(rhs));
        return *this;
    }

    friend Register operator*(Register lhs, const float& rhs) noexcept {
        lhs.data = _mm_mul_ps(lhs.data, _mm_set1_ps(rhs));
        return lhs;
    }

    Register& operator/=(const Register& rhs) noexcept {
        data = _mm_div_ps(data, rhs.data);
        return *this;
    }

    friend Register operator/(Register lhs, const Register& rhs) noexcept {
        lhs.data = _mm_div_ps(lhs.data, rhs.data);
        return lhs;
    }

    Register& operator/=(const float& rhs) noexcept {
        data = _mm_div_ps(data, _mm_set1_ps(rhs));
        return *this;
    }

    friend Register operator/(Register lhs, const float& rhs) noexcept {
        lhs.data = _mm_div_ps(lhs.data, _mm_set1_ps(rhs));
        return lhs;
    }
};
}// namespace HUH::Simd