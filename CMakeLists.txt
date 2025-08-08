cmake_minimum_required(VERSION 3.26)

project(HUH
        VERSION 0.1.1
        LANGUAGES CXX C
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_GRAPHICS "Build Graphics module" OFF)
option(BUILD_MATH "Build Math module" OFF)
option(USE_SIMD "Activates auto detection of cpu intrinsics and uses simd instructions where library supports it" ON)

option(BUILD_TESTS "Build google tests for the modules" OFF)
option(BUILD_BENCHMARKS "Build google benchmarks for the modules" OFF)

if (BUILD_TESTS)
    enable_testing()
endif ()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Tools/Cmake")
include(Tools/CpuCheck/CpuChecker.cmake)
check_cpu()


set(HEADER_FILES
        include/public/HUH/definitions.h
        include/public/HUH/types.h
)

set(SOURCE_FILES
)

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")

add_library(HUH OBJECT ${HEADER_FILES} ${SOURCE_FILES})

set_target_properties(HUH PROPERTIES LINKER_LANGUAGE CXX)
target_compile_definitions(HUH PUBLIC HUH_COMP)

if (USE_SIMD AND SSE_SUPPORT AND SSE2_SUPPORT)
    add_subdirectory(modules/Simd)
    huh_target_simd_compiler_flags(HUH INTERFACE)
    huh_target_simd_definitions(HUH INTERFACE)

    target_compile_definitions(HUH INTERFACE HUH_USE_SIMD)
endif ()

target_include_directories(HUH PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/public"
)


if (BUILD_MATH)
    add_subdirectory(modules/Math)
endif ()

if (BUILD_GRAPHICS)
    add_subdirectory(modules/Graphics)
endif ()
