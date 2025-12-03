find_package(Python COMPONENTS Interpreter Development.Module Development.Embed REQUIRED)
set(INC_PATH "include/HUH/Python")

set(HEADER_FILES
        ${INC_PATH}/bind.h
)

add_library(HUH-Python INTERFACE ${HEADER_FILES})
add_library(HUH::Python ALIAS HUH-Python)
target_link_libraries(HUH-Python INTERFACE
        Python::Python
        Python::Module
)


target_include_directories(HUH-Python INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")
