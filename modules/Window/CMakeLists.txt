set(WINDOW_INC_PATH "include/HUH/Window")
set(WINDOW_WINDOWS_INC_PATH "platform/include/HUH/Window/Windows")
set(LINUX_WINDOWS_INC_PATH "platform/include/HUH/Window/Linux")
set(WAYLAND_WINDOW_INC_PATH "platform/include/HUH/Window/Linux/Wayland")

set(HEADER_FILES
        ${WINDOW_INC_PATH}/prototypes/window_proto.h
        ${WINDOW_INC_PATH}/window.h
        ${WINDOW_INC_PATH}/definitions.h
)
set(SOURCE_FILES
        src/window_proto.cpp
)

set_property(TARGET HUH APPEND PROPERTY HUH_REQUIRED_INSTANCE_EXTENSIONS VK_KHR_surface VK_KHR_get_surface_capabilities2)
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
            ${LINUX_WINDOWS_INC_PATH}/window.h
    )
    list(APPEND SOURCE_FILES
            platform/src/linux/window.cpp
    )
endif ()

add_library(HUH-Window SHARED ${HEADER_FILES} ${SOURCE_FILES})
add_library(HUH::Window ALIAS HUH-Window)
huh_target_create_definition(HUH-Window)
target_link_libraries(HUH-Window PUBLIC
        HUH
        HUH::Core
        HUH::Math
)
target_include_directories(HUH-Window PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_SOURCE_DIR}/platform/include")


if (HUH_LINUX)
    if (TARGET HUH::WaylandConnector)
        set_property(TARGET HUH APPEND PROPERTY HUH_REQUIRED_INSTANCE_EXTENSIONS VK_KHR_wayland_surface)
        set(WAYLAND_HEADER_FILES
                ${WAYLAND_WINDOW_INC_PATH}/window.h
                ${WAYLAND_WINDOW_INC_PATH}/definitions.h
        )
        set(WAYLAND_SOURCE_FILES
                platform/src/linux/Wayland/window.cpp
        )
        add_library(HUH-WaylandWindow SHARED ${WAYLAND_SOURCE_FILES} ${WAYLAND_HEADER_FILES})
        add_library(HUH::WaylandWindow ALIAS HUH-WaylandWindow)
        huh_target_create_definition(HUH-WaylandWindow)
        target_link_libraries(HUH-WaylandWindow PUBLIC
                HUH
                HUH::Core
                HUH::Window
                HUH::WaylandConnector
        )
        target_include_directories(HUH-WaylandWindow PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_SOURCE_DIR}/platform/include")
    else ()
        message(WARNING "Wayland Connector target not build HUH::WaylandConnector")
    endif ()
endif ()


