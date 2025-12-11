# find wayland from: https://gist.github.com/mariobadr/acc3c8adf4b4e722705be38c3deac59a with little modifications
find_path(
        WAYLAND_CLIENT_INCLUDE_DIR
        NAMES wayland-client.h
)

find_library(
        WAYLAND_CLIENT_LIBRARY
        NAMES wayland-client libwayland-client
)

if (WAYLAND_CLIENT_INCLUDE_DIR AND WAYLAND_CLIENT_LIBRARY)
    add_library(Wayland::client UNKNOWN IMPORTED)

    set_target_properties(
            Wayland::client PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${WAYLAND_CLIENT_INCLUDE_DIR}"
            IMPORTED_LINK_INTERFACE_LANGUAGES "C"
            IMPORTED_LOCATION "${WAYLAND_CLIENT_LIBRARY}"
    )
endif ()

find_path(
        WAYLAND_SERVER_INCLUDE_DIR
        NAMES wayland-server.h
)

find_library(
        WAYLAND_SERVER_LIBRARY
        NAMES wayland-server libwayland-server
)

if (WAYLAND_SERVER_INCLUDE_DIR AND WAYLAND_SERVER_LIBRARY)
    add_library(Wayland::server UNKNOWN IMPORTED)

    set_target_properties(
            Wayland::server PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${WAYLAND_SERVER_INCLUDE_DIR}"
            IMPORTED_LINK_INTERFACE_LANGUAGES "C"
            IMPORTED_LOCATION "${WAYLAND_SERVER_LIBRARY}"
    )
endif ()

find_program(WAYLAND_SCANNER_EXECUTABLE NAMES wayland-scanner)
if (NOT TARGET Wayland::Scanner AND WAYLAND_SCANNER_EXECUTABLE)
    add_executable(Wayland::Scanner IMPORTED)
    set_target_properties(Wayland::Scanner PROPERTIES
            IMPORTED_LOCATION "${WAYLAND_SCANNER_EXECUTABLE}"
    )
endif ()

function(huh_wayland_create_protocol Target Visibility)
    set(options CLIENT SERVER PUBLIC_CODE PRIVATE_CODE)
    set(oneValue PROTOCOL FILENAME)
    cmake_parse_arguments(ARGS "${options}" "${oneValue}" "" ${ARGN})
    if (NOT ARGS_CLIENT AND NOT ARGS_SERVER)
        message(FATAL_ERROR "Wayland create protocol error: you must specify one of CLIENT SERVER")
    endif ()
    if (NOT ARGS_PUBLIC_CODE AND NOT ARGS_PRIVATE_CODE)
        message(FATAL_ERROR "Wayland create protocol error: you must specify one of PUBLIC PRIVATE")
    endif ()
    if (NOT ARGS_PROTOCOL)
        message(FATAL_ERROR "Wayland create protocol error: you must specify the path to the protocol from the root of WAYLAND_PROTOCOL_DIR variable")
    endif ()
    if (NOT ARGS_FILENAME)
        message(FATAL_ERROR "Wayland create protocol error: you must specify the base file name of the protocol")
    endif ()

    if (ARGS_PUBLIC_CODE)
        set(_PROTOCOL_TYPE "public-code")
    elseif (ARGS_PRIVATE_CODE)
        set(_PROTOCOL_TYPE "private-code")
    endif ()

    if (ARGS_CLIENT)
        set(_HEADER_TYPE "client-header")
        set(_HEADER_NAME "${CMAKE_CURRENT_BINARY_DIR}/generated/include/wayland-protocols/${ARGS_FILENAME}-client-protocol.h")
    elseif (ARGS_SERVER)
        set(_HEADER_TYPE "server-header")
        set(_HEADER_NAME "${CMAKE_CURRENT_BINARY_DIR}/generated/include/wayland-protocols/${ARGS_FILENAME}-server-protocol.h")
    endif ()

    set(_SOURCE_NAME "${CMAKE_CURRENT_BINARY_DIR}/generated/src/wayland-protocols/${ARGS_FILENAME}.c")

    add_custom_command(OUTPUT ${_HEADER_NAME}
            DEPENDS Wayland::Scanner "${WAYLAND_PROTOCOLS_DIR}/${ARGS_PROTOCOL}"
            COMMAND Wayland::Scanner
            ARGS ${_HEADER_TYPE} "${WAYLAND_PROTOCOLS_DIR}/${ARGS_PROTOCOL}" ${_HEADER_NAME}
    )

    add_custom_command(OUTPUT ${_SOURCE_NAME}
            DEPENDS Wayland::Scanner "${WAYLAND_PROTOCOLS_DIR}/${ARGS_PROTOCOL}"
            COMMAND Wayland::Scanner
            ARGS ${_PROTOCOL_TYPE} "${WAYLAND_PROTOCOLS_DIR}/${ARGS_PROTOCOL}" ${_SOURCE_NAME}
    )
    target_sources(${Target} ${Visibility} ${_HEADER_NAME} ${_SOURCE_NAME})


endfunction()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Wayland
        FOUND_VAR
        Wayland_FOUND
        REQUIRED_VARS
        WAYLAND_CLIENT_LIBRARY WAYLAND_CLIENT_INCLUDE_DIR WAYLAND_SERVER_LIBRARY WAYLAND_SERVER_INCLUDE_DIR WAYLAND_SCANNER_EXECUTABLE
)

mark_as_advanced(
        WAYLAND_CLIENT_INCLUDE_DIR
        WAYLAND_CLIENT_LIBRARY
        WAYLAND_SERVER_INCLUDE_DIR
        WAYLAND_SERVER_LIBRARY
        WAYLAND_SCANNER_EXECUTABLE
)