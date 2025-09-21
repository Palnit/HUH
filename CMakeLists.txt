cmake_minimum_required(VERSION 3.26)

option(BUILD_GRAPHICS "Build Graphics module" OFF)
option(BUILD_MATH "Build Math module" OFF)
option(BUILD_ARRAY "Build numpy like array module" OFF)
option(BUILD_THREADING "Build threading library" OFF)
option(USE_SIMD "Activates auto detection of cpu intrinsics and uses simd instructions where library supports it" ON)

option(BUILD_TESTS "Build google tests for the modules" OFF)
option(BUILD_BENCHMARKS "Build google benchmarks for the modules" OFF)
option(GENERATE_DOCS "Generate doxygen documentation" OFF)

project(HUH
        VERSION 0.1.1
        LANGUAGES CXX C
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


if (BUILD_TESTS)
    enable_testing()
endif ()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Tools/Cmake")
include(HUHDependency)
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
if (WIN32)
    set(HUH_WIN yes)
    target_compile_definitions(HUH PUBLIC HUH_WIN)
elseif (UNIX AND NOT APPLE)
    set(HUH_LINUX yes)
    target_compile_definitions(HUH PUBLIC HUH_LINUX)
endif ()

target_include_directories(HUH PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include/public"
)

if (USE_SIMD AND SSE_SUPPORT AND SSE2_SUPPORT)
    message(STATUS "Configuring ${MODULE}")
    huh_target_simd_compiler_flags(HUH INTERFACE)
    huh_target_simd_definitions(HUH INTERFACE)

    add_subdirectory(modules/Simd)

    target_compile_definitions(HUH INTERFACE HUH_USE_SIMD)
endif ()

add_subdirectory(modules/Core)

huh_set_modules_dependency(HUH)

if (BUILD_MATH)
    huh_add_module_build(HUH INTERFACE Math)
endif ()

if (BUILD_GRAPHICS)
    huh_add_module_build(HUH INTERFACE Graphics)
endif ()

if (BUILD_ARRAY)
    huh_add_module_build(HUH INTERFACE Array)
endif ()

if (BUILD_THREADING)
    huh_add_module_build(HUH INTERFACE Threading)
endif ()

get_property(BUILD_MODULES TARGET HUH PROPERTY BUILD_MODULES)

foreach (MODULE IN LISTS BUILD_MODULES)
    message(STATUS "Configuring ${MODULE}")
    add_subdirectory(${MODULE})
endforeach ()

if (GENERATE_DOCS)
    add_subdirectory(Tools/docs)
endif ()

