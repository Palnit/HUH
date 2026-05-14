# Based on the kde Kwin find xkb: https://github.com/KDE/kwin/blob/master/cmake/modules/FindXKB.cmake#L71 with modifications:

#.rst:
# FindXKB
# -------
#
# Try to find xkbcommon on a Unix system
# If found, this will define the following variables:
#
#   ``XKB_FOUND``
#     True if XKB is available
#   ``XKB_LIBRARIES``
#     Link these to use XKB
#   ``XKB_INCLUDE_DIRS``
#     Include directory for XKB
#   ``XKB_DEFINITIONS``
#     Compiler flags for using XKB
#
# Additionally, the following imported targets will be defined:
#
#   ``XKB::XKB``
#     The XKB library

#=============================================================================
# SPDX-FileCopyrightText: 2014 Martin Gräßlin <mgraesslin@kde.org>
#
# SPDX-License-Identifier: BSD-3-Clause
#=============================================================================

find_package(PkgConfig)
pkg_check_modules(PKG_XKB QUIET xkbcommon)

set(XKB_DEFINITIONS ${PKG_XKB_CFLAGS_OTHER})

find_path(XKB_INCLUDE_DIR
        NAMES
        xkbcommon/xkbcommon.h
        HINTS
        ${PKG_XKB_INCLUDE_DIRS}
)
find_library(XKB_LIBRARY
        NAMES
        xkbcommon
        HINTS
        ${PKG_XKB_LIBRARY_DIRS}
)

set(XKB_LIBRARIES ${XKB_LIBRARY})
set(XKB_INCLUDE_DIRS ${XKB_INCLUDE_DIR})
set(XKB_VERSION ${PKG_XKB_VERSION})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(XKB
        FOUND_VAR
        XKB_FOUND
        REQUIRED_VARS
        XKB_LIBRARY
        XKB_INCLUDE_DIR
        VERSION_VAR
        XKB_VERSION
)

if (XKB_FOUND AND NOT TARGET XKB::XKB)
    add_library(XKB::XKB UNKNOWN IMPORTED)
    set_target_properties(XKB::XKB PROPERTIES
            IMPORTED_LOCATION "${XKB_LIBRARY}"
            INTERFACE_COMPILE_OPTIONS "${XKB_DEFINITIONS}"
            INTERFACE_INCLUDE_DIRECTORIES "${XKB_INCLUDE_DIR}"
    )
endif ()

include(FeatureSummary)
set_package_properties(XKB PROPERTIES
        URL "https://xkbcommon.org"
        DESCRIPTION "XKB API common to servers and clients"
)
mark_as_advanced(
        XKB_LIBRARY
        XKB_DEFINITIONS
        XKB_INCLUDE_DIR
)
