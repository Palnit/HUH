set(MATH_INC_PATH "include/public/HUH/Math")

set(HEADER_FILES
        ${MATH_INC_PATH}/vector.h
        ${MATH_INC_PATH}/vector/vector2.inl.h
        ${MATH_INC_PATH}/vector/vector3.inl.h
        ${MATH_INC_PATH}/vector/vector4.inl.h
        ${MATH_INC_PATH}/matrix.h
        ${MATH_INC_PATH}/matrix_functions.h
        ${MATH_INC_PATH}/vector_functions.h
        ${MATH_INC_PATH}/fwd.h
        ${MATH_INC_PATH}/matrix/matrix_4x4.inl.h
)


add_library(HUH-Math INTERFACE ${HEADER_FILES})
add_library(HUH::Math ALIAS HUH-Math)
#target_compile_definitions(HUH-Math PRIVATE HUH_COMP)

set_target_properties(HUH-Math PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(HUH-Math INTERFACE HUH)

if (USE_SIMD)
    target_link_libraries(HUH-Math INTERFACE HUH::Simd)
endif ()


target_include_directories(HUH-Math INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include/public")


if (BUILD_TESTS)
    add_subdirectory(tests)
endif ()

if (BUILD_BENCHMARKS)
    add_subdirectory(benchmarks)
endif ()
