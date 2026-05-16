cmake_minimum_required(VERSION 3.28)

option(BUILD_RHI "Build Render Hardware Interface module" OFF)
option(BUILD_WINDOW "Build Window management module" OFF)
option(BUILD_VULKAN_LOADER "Build Vulkan Loader" OFF)
option(BUILD_MATH "Build Math module" OFF)
option(BUILD_ARRAY "Build numpy like array module" OFF)
option(BUILD_THREADING "Build threading library" OFF)
option(BUILD_PYTHON_BINDING "Build the python binding module" OFF)
option(BUILD_HUH_PYTHON_BINDING "Build the python binding for all building HUH module" OFF)
option(BUILD_WAYLAND_CONNECTOR "Build the wayland connector abstraction layer to simplify wayland connections on linux" OFF)
option(USE_SIMD "Activates auto detection of cpu intrinsics and uses simd instructions where library supports it" ON)

#Graphics
option(BUILD_GRAPHICS_ALL "Builds all graphics modules" OFF)
option(BUILD_GRAPHICS_CAMERA "Builds a camera module" OFF)


option(BUILD_TESTS "Build google tests for the modules" OFF)
option(BUILD_BENCHMARKS "Build google benchmarks for the modules" OFF)
option(GENERATE_DOCS "Generate doxygen documentation" OFF)


project(HUH
        VERSION 0.1.1
        LANGUAGES CXX C
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


if (BUILD_TESTS)
    enable_testing()
endif ()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Tools/Cmake")
include(HUHDependency)
include(HUHDefine)
include(Tools/CpuCheck/CpuChecker.cmake)
check_cpu()


set(HEADER_FILES
        include/public/HUH/definitions.h
        include/public/HUH/types.h
        include/public/HUH/templates.h
        include/public/HUH/concepts.h
        include/public/HUH/enum_define.h
        include/public/HUH/enum_helper.h
)

set(SOURCE_FILES
)

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(HUH_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

add_library(HUH OBJECT ${HEADER_FILES} ${SOURCE_FILES})

set_target_properties(HUH PROPERTIES LINKER_LANGUAGE CXX)
target_compile_features(HUH PUBLIC cxx_std_20)
if (WIN32)
    set(HUH_WIN yes)
    target_compile_definitions(HUH PUBLIC HUH_WIN)
elseif (UNIX AND NOT APPLE)
    set(HUH_LINUX yes)
    target_compile_definitions(HUH PUBLIC HUH_LINUX)
endif ()

target_compile_definitions(HUH PUBLIC HUH_DEBUG $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:DEBUG> HUH_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

target_include_directories(HUH PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include/public"
)

if (USE_SIMD AND SSE_SUPPORT AND SSE2_SUPPORT)
    message(STATUS "Configuring modules/Simd")
    huh_target_simd_compiler_flags(HUH INTERFACE)
    huh_target_simd_definitions(HUH INTERFACE)

    add_subdirectory(modules/Simd)

    target_compile_definitions(HUH INTERFACE HUH_USE_SIMD)
endif ()

huh_set_modules_dependency(HUH)

add_subdirectory(modules/Core)


if (BUILD_MATH)
    huh_add_module_build(HUH INTERFACE Math)
endif ()


if (BUILD_ARRAY)
    huh_add_module_build(HUH INTERFACE Array)
endif ()

if (BUILD_THREADING)
    huh_add_module_build(HUH INTERFACE Threading)
endif ()

if (BUILD_WINDOW)
    huh_add_module_build(HUH INTERFACE Window)
endif ()

if (BUILD_RHI)
    huh_add_module_build(HUH INTERFACE RHI)
endif ()

if (BUILD_VULKAN_LOADER)
    huh_add_module_build(HUH INTERFACE VulkanLoader)
endif ()

if (BUILD_PYTHON_BINDING)
    huh_add_module_build(HUH INTERFACE Python)
endif ()

if (BUILD_WAYLAND_CONNECTOR)
    huh_add_module_build(HUH INTERFACE WaylandConnector)
endif ()

add_subdirectory(modules/Graphics)


get_property(BUILD_MODULES TARGET HUH PROPERTY BUILD_MODULES)

foreach (MODULE IN LISTS BUILD_MODULES)
    message(STATUS "Configuring ${MODULE}")
    add_subdirectory(${MODULE})
endforeach ()

if (GENERATE_DOCS)
    add_subdirectory(Tools/docs)
endif ()

get_property(HUH_LIB_IMPORT_DEFINES TARGET HUH PROPERTY HUH_LIB_IMPORT_DEFINES)
foreach (LIB_IMPORT IN LISTS HUH_LIB_IMPORT_DEFINES)

endforeach ()

get_property(HUH_LIB_IMPORT_DEFINES TARGET HUH PROPERTY HUH_LIB_IMPORT_DEFINES)
if (HUH_LIB_IMPORT_DEFINES)
    list(GET HUH_LIB_IMPORT_DEFINES 0 temp)
    list(LENGTH HUH_LIB_IMPORT_DEFINES LIB_LENGTH)
    math(EXPR N "${LIB_LENGTH}-1")
    foreach (IDX RANGE 1 ${N})
        list(GET HUH_LIB_IMPORT_DEFINES ${IDX} STR)
        set(temp "${temp}\n${STR}")
    endforeach ()
    set_property(TARGET HUH PROPERTY HUH_LIB_IMPORT_DEFINES ${temp})
endif ()

file(GENERATE OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/include/public/HUH/lib_defines.h INPUT configure/lib_defines.h.ini)

