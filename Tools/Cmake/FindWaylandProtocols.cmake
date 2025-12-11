find_package(PkgConfig QUIET)
pkg_check_modules(PKG_wayland_protocols QUIET wayland-protocols)

set(WaylandProtocols_VERSION ${PKG_wayland_protocols_VERSION})
pkg_get_variable(WAYLAND_PROTOCOLS_DIR wayland-protocols pkgdatadir)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(WaylandProtocols
        FOUND_VAR WaylandProtocols_FOUND
        REQUIRED_VARS WAYLAND_PROTOCOLS_DIR
        VERSION_VAR WaylandProtocols_VERSION
)
