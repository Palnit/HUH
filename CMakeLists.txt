cmake_minimum_required(VERSION 3.26)

project(HUH
        VERSION 0.1.1
        LANGUAGES CXX C
)

set(CMAKE_CXX_STANDARD 17)

set(HEADER_FILES
        include/public/HUH/huh_defs.h
)

set(SOURCE_FILES
)

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(implot CONFIG REQUIRED)

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")

add_library(HUH OBJECT ${HEADER_FILES} ${SOURCE_FILES})
include(GenerateExportHeader)
generate_export_header(HUH BASE_NAME HUH EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/generated/include/HUH/export.h" )
set_target_properties(HUH PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(HUH PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/public"
        "${CMAKE_CURRENT_BINARY_DIR}/generated/include"
)

add_subdirectory(modules/Graphics)