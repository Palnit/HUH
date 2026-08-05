option(SHARED_LIB_LOAD_WITH_RELATIVE_PATH "Try-s to load every shared library relative to exetable" OFF)
set(CORE_INC_PATH "include/HUH")
set(CORE_LINUX_INC_PATH "platform/include/HUH/Linux")
set(CORE_WIN_INC_PATH "platform/include/HUH/Windows")

set(CORE_HEADER_FILE
        ${CORE_INC_PATH}/Singleton.h
        ${CORE_INC_PATH}/array.h
        ${CORE_INC_PATH}/views.h
        ${CORE_INC_PATH}/Allocators/base_allocator.h
        ${CORE_INC_PATH}/Allocators/malloc_allocator.h
        ${CORE_INC_PATH}/dynamic_library.h
        ${CORE_INC_PATH}/logging.h
        ${CORE_INC_PATH}/string_operations.h
        ${CORE_INC_PATH}/console_color.h
        ${CORE_INC_PATH}/event.h
        ${CORE_INC_PATH}/memory_movement.h
        ${CORE_INC_PATH}/Types/tuple.h
)
set(CORE_SOURCE_FILES
        src/logging.cpp
        src/string_operations.cpp
        src/event.cpp
        src/console_color.cpp
)

if (HUH_WIN)
    list(APPEND CORE_HEADER_FILES
            ${CORE_WIN_INC_PATH}/dynamic_library.h
            ${CORE_WIN_INC_PATH}/win_main.h
            ${CORE_WIN_INC_PATH}/win_instance.h
    )
    list(APPEND CORE_SOURCE_FILES
            platform/src/Windows/dynamic_library.cpp
            platform/src/Windows/win_instance.cpp
    )
elseif (HUH_LINUX)
    list(APPEND CORE_HEADER_FILES
            ${CORE_LINUX_INC_PATH}/dynamic_library.h
    )
    list(APPEND CORE_SOURCE_FILES
            platform/src/Linux/dynamic_library.cpp
    )
endif ()

add_library(HUH-Core SHARED ${CORE_HEADER_FILES} ${CORE_SOURCE_FILES})
add_library(HUH::Core ALIAS HUH-Core)
huh_target_compile_options(HUH-Core PUBLIC MSVC /Zc:preprocessor)
if (HUH_LINUX)
    target_link_libraries(HUH-Core PUBLIC ${CMAKE_DL_LIBS})
endif ()

set_target_properties(HUH-Core PROPERTIES LINKER_LANGUAGE CXX)
huh_target_create_definition(HUH-Core)

target_link_libraries(HUH-Core PUBLIC HUH)

target_include_directories(HUH-Core PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include/"
        "${CMAKE_CURRENT_SOURCE_DIR}/platform/include"
)

if (BUILD_BENCHMARKS)
    add_subdirectory(benchmarks)
endif ()

