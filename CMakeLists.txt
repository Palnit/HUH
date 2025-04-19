cmake_minimum_required(VERSION 3.26)

project(HUH
        VERSION 0.1.1
        LANGUAGES CXX C
)

set(CMAKE_CXX_STANDARD 17)

option(BUILD_GRAPHICS "Build Graphics module" OFF)

set(HEADER_FILES
        include/public/HUH/definitions.h
)

set(SOURCE_FILES
)

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")

add_library(HUH OBJECT ${HEADER_FILES} ${SOURCE_FILES})

set_target_properties(HUH PROPERTIES LINKER_LANGUAGE CXX)
target_compile_definitions(HUH PUBLIC HUH_COMP)

target_include_directories(HUH PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/public"
)

if (BUILD_GRAPHICS)
    add_subdirectory(modules/Graphics)
endif ()