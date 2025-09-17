set(CORE_INC_PATH "include/HUH")

set(HEADER_FILES)
set(SOURCE_FILES)


add_library(HUH-Core INTERFACE ${HEADER_FILES} ${SOURCE_FILES} )
add_library(HUH::Core ALIAS HUH-Core)

set_target_properties(HUH-Core PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(HUH-Core INTERFACE HUH)


target_include_directories(HUH PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include/"
)
