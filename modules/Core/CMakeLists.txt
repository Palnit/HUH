set(CORE_INC_PATH "include/HUH")
set(CORE_LINUX_INC_PATH "platform/include/HUH/Linux")

set(CORE_HEADER_FILE
        ${CORE_INC_PATH}/Singleton.h
        ${CORE_INC_PATH}/dynamic_library.h
)
set(CORE_SOURCE_FILES)

if (HUH_WIN)
elseif (HUH_LINUX)
    list(APPEND CORE_HEADER_FILES ${CORE_LINUX_INC_PATH}/dynamic_library.h)
    list(APPEND CORE_SOURCE_FILES platform/src/Linux/dynamic_library.cpp)
endif ()

add_library(HUH-Core SHARED ${CORE_HEADER_FILES} ${CORE_SOURCE_FILES})
add_library(HUH::Core ALIAS HUH-Core)

set_target_properties(HUH-Core PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(HUH-Core PUBLIC HUH)

target_compile_definitions(HUH-Core PRIVATE HUH_COMP)


target_include_directories(HUH-Core PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include/"
        "${CMAKE_CURRENT_SOURCE_DIR}/platform/include"
)

