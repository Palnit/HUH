find_package(Doxygen COMPONENTS dot)

if (Doxygen_FOUND)
    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_GENERATE_XML YES)
    set(DOXYGEN_XML_OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/docs_site/src/lib/assets/doxygen)
    set(DOXYGEN_XML_NS_MEMB_FILE_SCOPE YES)
    set(DOXYGEN_PROJECT_NAME "Helpful Ultra Hoarder")
    set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    set(DOXYGEN_GENERATE_TREEVIEW YES)
    set(DOXYGEN_DISABLE_INDEX NO)
    set(DOXYGEN_FULL_SIDEBAR NO)
    set(DOXYGEN_PAGE_OUTLINE_PANEL NO)

    list(APPEND DOXYGEN_EXCLUDE_PATTERNS "*/tests/*" "*/benchmarks/*")

    get_property(BUILD_MODULES TARGET HUH PROPERTY BUILD_MODULES)


    set(DOXY_MODULES "")
    foreach (MODULE IN LISTS BUILD_MODULES)
        list(APPEND DOXY_MODULES ../../${MODULE}/include)
    endforeach ()
    doxygen_add_docs(HUH-Docs ${DOXY_MODULES} ALL)
else ()
    message(STATUS "Doxygen not found skipping Documentation generation")
    set(GENERATE_DOCS OFF)
endif ()


