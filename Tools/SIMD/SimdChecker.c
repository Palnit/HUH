#if defined(__clang__)
#define HAS_CPUID_COUNT 1
#include <cpuid.h>
#elif defined(__GNUC__)
#define HAS_CPUIDEX 1
#include <cpuid.h>
#elif defined(_MSC_VER)
#define HAS_CPUIDEX 1
#include <intrin.h>
#endif

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct cpu_featureset {
    bool ext_sse3;
    bool ext_ssse3;
    bool ext_fma;
    bool ext_sse4_1;
    bool ext_sse4_2;
    bool ext_avx;
    bool ext_mmx;
    bool ext_sse;
    bool ext_sse2;
    bool ext_avx2;
    bool ext_avx512;
} cpu_featureset;

typedef struct cpuid_result {
    int eax;
    int ebx;
    int ecx;
    int edx;
} cpuid_result;

cpuid_result call_cpuid(int function, int subfunction) {
    cpuid_result result = {};
    memset(&result, 0, sizeof(cpuid_result));

#if HAS_CPUID_COUNT
    __cpuid_count(function, subfunction, result.eax, result.ebx, result.ecx, result.edx);
#elif HAS_CPUIDEX
    __cpuidex((int*) &result, function, subfunction);
#else
#error No cpuid intrinsic available
#endif

    return result;
}

cpu_featureset detect_cpu_features() {
    cpu_featureset features = {};
    memset(&features, 0, sizeof(cpu_featureset));

    features.ext_sse3 = call_cpuid(0x1, 0x0).ecx & 0b1;
    features.ext_ssse3 = call_cpuid(0x1, 0x0).ecx & (0b1 << 9);
    features.ext_fma = call_cpuid(0x1, 0x0).ecx & (0b1 << 12);
    features.ext_sse4_1 = call_cpuid(0x1, 0x0).ecx & (0b1 << 19);
    features.ext_sse4_2 = call_cpuid(0x1, 0x0).ecx & (0b1 << 20);
    features.ext_avx = call_cpuid(0x1, 0x0).ecx & (0b1 << 28);
    features.ext_mmx = call_cpuid(0x1, 0x0).edx & (0b1 << 23);
    features.ext_sse = call_cpuid(0x1, 0x0).edx & (0b1 << 25);
    features.ext_sse2 = call_cpuid(0x1, 0x0).edx & (0b1 << 26);
    features.ext_avx2 = call_cpuid(0x7, 0x0).ebx & (0b1 << 5);
    features.ext_avx512 = call_cpuid(0xD, 0x0).eax & (0b1 << 7);

    return features;
}

const char* get_cpu_brand_string() {
    int view_size = sizeof(int) * 4;
    int buffer_size = view_size * 3;

    char* buffer = malloc(buffer_size);
    memset(buffer, 0, buffer_size);

    for (int i = 0; i < 3; i++) {
        cpuid_result result = call_cpuid(0x80000002 + i, 0x0);
        memcpy(buffer + view_size * i, &result, view_size);
    }

    return buffer;
}

const char* get_cpu_vendor_string() {
    int buffer_size = sizeof(int) * 3;

    char* buffer = malloc(buffer_size);
    memset(buffer, 0, buffer_size);

    cpuid_result result = call_cpuid(0x0, 0x0);
    memcpy(buffer, &result.ebx, sizeof(int));
    memcpy(buffer + sizeof(int), &result.edx, sizeof(int));
    memcpy(buffer + sizeof(int) * 2, &result.ecx, sizeof(int));

    return buffer;
}

void pretty_print_feature(const char* name, bool available) { printf("%s:\t%s\n", name, available ? "yes" : "no"); }

void pretty_print_features(cpu_featureset features) {
    pretty_print_feature("SSE3", features.ext_sse3);
    pretty_print_feature("SSSE3", features.ext_ssse3);
    pretty_print_feature("FMA", features.ext_ssse3);
    pretty_print_feature("SSE4.1", features.ext_sse4_1);
    pretty_print_feature("SSE4.2", features.ext_sse4_2);
    pretty_print_feature("AVX", features.ext_avx);
    pretty_print_feature("MMX", features.ext_mmx);
    pretty_print_feature("SSE", features.ext_sse);
    pretty_print_feature("SSE2", features.ext_sse2);
    pretty_print_feature("AVX2", features.ext_avx2);
    pretty_print_feature("AVX512", features.ext_avx512);
}

int main(int argc, char** argv) {
    const char* vendor_name = get_cpu_vendor_string();
    const char* cpu_name = get_cpu_brand_string();
    printf("%s %s\n", vendor_name, cpu_name);
    free((void*) cpu_name);
    free((void*) vendor_name);

    cpu_featureset features = detect_cpu_features();
    pretty_print_features(features);
    return 0;
}
