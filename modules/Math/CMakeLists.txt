set(MATH_INC_PATH "include/public/HUH/Math")

set(HEADER_FILES
        ${MATH_INC_PATH}/vector.h
        ${MATH_INC_PATH}/vector/Vec2/vector2.inl.h
        ${MATH_INC_PATH}/vector/Vec3/vector3.inl.h
        ${MATH_INC_PATH}/vector/Vec4/vector4.inl.h
        ${MATH_INC_PATH}/matrix.h
)

if (USE_SIMD)
    list(APPEND HEADER_FILES
            ${MATH_INC_PATH}/vector/Vec2/Simd/vector2_float.inl.h
    )
endif ()

add_library(HUH-Math INTERFACE ${HEADER_FILES})
add_library(HUH::Math ALIAS HUH-Math)
find_package(TBB CONFIG REQUIRED)

set_target_properties(HUH-Math PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(HUH-Math INTERFACE TBB::tbb TBB::tbbmalloc)


target_include_directories(HUH-Math INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include/public")


if (BUILD_TESTS)
    add_subdirectory(Tests)
endif ()

