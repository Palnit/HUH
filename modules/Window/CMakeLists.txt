set(WINDOW_INC_PATH "include/HUH/Window")
set(WINDOW_WINDOWS_INC_PATH "platform/include/HUH/Window/Windows")

set(HEADER_FILES
        ${WINDOW_INC_PATH}/prototypes/window_proto.h
        ${WINDOW_INC_PATH}/window.h
        ${WINDOW_INC_PATH}/definitions.h
)
set(SOURCE_FILES
)

set_property(TARGET HUH APPEND PROPERTY HUH_REQUIRED_INSTANCE_EXTENSIONS VK_KHR_surface)
set_property(TARGET HUH APPEND PROPERTY HUH_REQUIRED_DEVICE_EXTENSIONS VK_KHR_swapchain)

if (HUH_WIN)
    set_property(TARGET HUH APPEND PROPERTY HUH_REQUIRED_INSTANCE_EXTENSIONS VK_KHR_win32_surface)
    list(APPEND HEADER_FILES
            ${WINDOW_WINDOWS_INC_PATH}/window.h
    )
    list(APPEND SOURCE_FILES
            platform/src/Windows/window.cpp
    )
elseif (HUH_LINUX)
    list(APPEND HEADER_FILES
    )
    list(APPEND SOURCE_FILES)
endif ()


add_library(HUH-Window SHARED ${HEADER_FILES} ${SOURCE_FILES})
add_library(HUH::Window ALIAS HUH-Window)
huh_target_create_definition(HUH-Window)
target_link_libraries(HUH-Window PUBLIC
        HUH
        HUH::Core
)



target_include_directories(HUH-Window PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_SOURCE_DIR}/platform/include")
