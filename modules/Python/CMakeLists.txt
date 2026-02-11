find_package(Python COMPONENTS Interpreter Development.Module Development.Embed REQUIRED)
set(INC_PATH "include/HUH/Python")

set(HEADER_FILES
        ${INC_PATH}/definitions.h
        ${INC_PATH}/module.h
        ${INC_PATH}/class.h
        ${INC_PATH}/function.h
        ${INC_PATH}/cast.h
        ${INC_PATH}/Types/accessor.h
        ${INC_PATH}/Types/object.h
        ${INC_PATH}/Types/tuple.h
        ${INC_PATH}/Types/unicode.h
        ${INC_PATH}/Types/fwd.h
)

add_library(HUH-Python INTERFACE ${HEADER_FILES})
add_library(HUH::Python ALIAS HUH-Python)
set_target_properties(HUH-Python PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(HUH-Python INTERFACE
        HUH
        HUH::Core
        Python::Python
        Python::Module
)

target_include_directories(HUH-Python INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")
