/**
 * Fast range reduction without division.
 *
 * Based on the fastrange implementation by Daniel Lemire:
 * https://github.com/lemire/fastrange
 *
 * Algorithm reference:
 * https://lemire.me/blog/2016/06/27/a-fast-alternative-to-the-modulo-reduction/
 *
 * Original copyright:
 * Copyright (c) Daniel Lemire
 *
 * Modifications:
 * Copyright (c) 2026 Palnit
 *
 * This file has been modified from the original implementation.
 * Licensed under the Apache License, Version 2.0.
 * See the project's LICENSE file for the full license text.
 */
#pragma once

#include <HUH/definitions.h>
#include <HUH/types.h>

#if defined(_MSC_VER) && defined(HUH_WIN)
#include <intrin.h>// should be part of all recent Visual Studio
#pragma intrinsic(_umul128)
#endif

namespace HUH {

HUH_NODISCARD HUH_CONSTEXPR_FORCE Uint8 FastRange(const Uint8 word, const Uint8 p) {
    return static_cast<Uint8>((static_cast<Uint16>(word) * static_cast<Uint16>(p)) >> 8);
}

HUH_NODISCARD HUH_CONSTEXPR_FORCE Uint16 FastRange(const Uint16 word, const Uint16 p) {
    return static_cast<Uint16>((static_cast<Uint32>(word) * static_cast<Uint32>(p)) >> 16);
}

HUH_NODISCARD HUH_CONSTEXPR_FORCE Uint32 FastRange(Uint32 word, Uint32 p) {
    return static_cast<Uint32>((static_cast<Uint64>(word) * static_cast<Uint64>(p)) >> 32);
}

HUH_NODISCARD HUH_CONSTEXPR_FORCE Uint64 FastRange(Uint64 word, Uint64 p) {
#if defined(__SIZEOF_INT128__)
    return static_cast<Uint64>((static_cast<__uint128_t>(word) * static_cast<__uint128_t>(p)) >> 64);
#elif defined(_MSC_VER) && defined(HUH_WIN)
    Uint64 highProduct;
    _umul128(word, p, &highProduct);
    return highProduct;
#else
    return word % p;
#endif
}

template<HUH::Integral T>
    requires(!HUH::Same<T, bool>)
HUH_NODISCARD HUH_CONSTEXPR_FORCE T FastRange(T word, T p) {
    using UT = std::make_unsigned_t<T>;
    return static_cast<T>(FastRange(static_cast<UT>(word), static_cast<UT>(p)));
}

}// namespace HUH
