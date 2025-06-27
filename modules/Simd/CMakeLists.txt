set(SIMD_INC_PATH "include/public/HUH/Simd")

set(HEADER_FILES
        ${SIMD_INC_PATH}/register.h
        ${SIMD_INC_PATH}/shuffle.h
        ${SIMD_INC_PATH}/hsum.h
        ${SIMD_INC_PATH}/register/register_float4.h
)

add_library(HUH-Simd INTERFACE ${HEADER_FILES})
add_library(HUH::Simd ALIAS HUH-Simd)

set_target_properties(HUH-Simd PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(HUH-Simd INTERFACE HUH)


target_include_directories(HUH-Simd INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include/public")


#if (BUILD_TESTS)
#    add_subdirectory(Tests)
#endif ()
#
#if (BUILD_BENCHMARKS)
#    add_subdirectory(benchmarks)
#endif ()
