if (HUH_LINUX)
    find_package(Wayland)
    find_package(WaylandProtocols)
    find_package(XKB)
    if (Wayland_FOUND AND WaylandProtocols_FOUND AND XKB_FOUND)
        set(WAYLAND_CONNECTOR_INC_PATH "include/HUH")
        set(WAYLAND_CONNECTOR_HEADER_FILES
                ${WAYLAND_CONNECTOR_INC_PATH}/wayland_connector.h
        )
        set(WAYLAND_CONNECTOR_SOURCE_FILES
                src/wayland_connector.cpp
        )
        add_library(HUH-WaylandConnector SHARED ${WAYLAND_CONNECTOR_SOURCE_FILES} ${WAYLAND_CONNECTOR_HEADER_FILES})
        add_library(HUH::WaylandConnector ALIAS HUH-WaylandConnector)
        huh_wayland_create_protocol(HUH-WaylandConnector PUBLIC CLIENT PRIVATE_CODE PROTOCOLS_DIR ${WAYLAND_PROTOCOLS_DIR} PROTOCOL "stable/xdg-shell/xdg-shell.xml" FILENAME "xdg-shell")
        huh_wayland_create_protocol(HUH-WaylandConnector PUBLIC CLIENT PRIVATE_CODE PROTOCOLS_DIR ${WAYLAND_PROTOCOLS_DIR} PROTOCOL "unstable/xdg-decoration/xdg-decoration-unstable-v1.xml" FILENAME "xdg-decoration")
        find_package(PlasmaWaylandProtocols)
        if (PlasmaWaylandProtocols_FOUND)
            target_compile_definitions(HUH-WaylandConnector PUBLIC HUH_PLASMA_PROTOCOLS)
        endif ()
        huh_target_create_definition(HUH-WaylandConnector)
        target_link_libraries(HUH-WaylandConnector PUBLIC
                HUH
                HUH::Core
                Wayland::client
                XKB::XKB
        )
        target_include_directories(HUH-WaylandConnector PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}/generated/include")
    else ()
        message(WARNING "WaylandConnector Build Failed Cannot find wayland library or protocols: Wayland_FOUND: ${Wayland_FOUND}, WaylandProtocols_FOUND: ${WaylandProtocols_FOUND}")
    endif ()
else ()
    message(WARNING "WaylandConnector Build turned on but none linux platform skipping configuration")
endif ()
