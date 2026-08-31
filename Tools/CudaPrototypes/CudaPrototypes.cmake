function(get_transitive_shared_library_names TARGET OUT_VAR)
    set(_visited "")
    set(_libraries "")

    function(_walk CURRENT)
        if (NOT TARGET "${CURRENT}")
            return()
        endif ()

        if ("${CURRENT}" IN_LIST _visited)
            return()
        endif ()

        list(APPEND _visited "${CURRENT}")

        get_target_property(_type "${CURRENT}" TYPE)

        if (_type MATCHES "^(STATIC_LIBRARY|SHARED_LIBRARY|MODULE_LIBRARY|OBJECT_LIBRARY|INTERFACE_LIBRARY)$")
            list(APPEND _libraries "${CURRENT}")
        endif ()

        get_target_property(_links "${CURRENT}" LINK_LIBRARIES)
        get_target_property(_interface_links "${CURRENT}" INTERFACE_LINK_LIBRARIES)

        foreach (_link IN LISTS _links _interface_links)
            if (TARGET "${_link}")
                _walk("${_link}")
            endif ()
        endforeach ()
        set(_libraries ${_libraries} PARENT_SCOPE)
    endfunction()

    _walk("${TARGET}")

    set(_names "")
    foreach (_library IN LISTS _libraries)
        list(APPEND _names "$<IF:$<IN_LIST:$<TARGET_PROPERTY:${_library},TYPE>,EXECUTABLE;STATIC_LIBRARY;SHARED_LIBRARY;MODULE_LIBRARY;OBJECT_LIBRARY>,$<TARGET_OBJECTS:${_library}>,>$<IF:$<IN_LIST:$<TARGET_PROPERTY:${_library},TYPE>,STATIC_LIBRARY;SHARED_LIBRARY;MODULE_LIBRARY;EXECUTABLE>,;$<TARGET_FILE_NAME:${_library}>,>")
    endforeach ()

    set(${OUT_VAR} "${_names}" PARENT_SCOPE)
endfunction()

function(get_all_public_include_directories TARGET OUTPUT_VAR)
    set(_visited "")
    set(_result "")

    function(_collect TARGET)
        if ("${TARGET}" IN_LIST _visited)
            return()
        endif ()

        list(APPEND _visited "${TARGET}")
        set(_visited "${_visited}" PARENT_SCOPE)

        get_target_property(_includes
                "${TARGET}"
                INTERFACE_INCLUDE_DIRECTORIES
        )

        if (_includes)
            list(APPEND _result ${_includes})
        endif ()


        get_target_property(_links "${TARGET}" LINK_LIBRARIES)
        get_target_property(_interface_links "${TARGET}" INTERFACE_LINK_LIBRARIES)

        foreach (_link IN LISTS _links _interface_links)
            if (TARGET "${_link}")
                _collect("${_link}")
            endif ()
        endforeach ()

        set(_result "${_result}" PARENT_SCOPE)

    endfunction()

    _collect("${TARGET}")

    list(REMOVE_DUPLICATES _result)
    set("${OUTPUT_VAR}" "${_result}" PARENT_SCOPE)
endfunction()

function(huh_create_prototype)
    set(single_args TARGET)
    cmake_parse_arguments(PARSE_ARGV 0 CreateOptions "" "${single_args}" "")
    if (NOT DEFINED CreateOptions_TARGET)
        message(FATAL_ERROR "huh_create_prototype needs a cuda TARGET")
    endif ()
    find_package(Clang CONFIG REQUIRED)
    execute_process(
            COMMAND clang -print-resource-dir
            OUTPUT_VARIABLE CLANG_RESOURCE_DIR
            OUTPUT_STRIP_TRAILING_WHITESPACE
            RESULT_VARIABLE CLANG_RESOURCE_DIR_RESULT
    )
    if (NOT TARGET HUHCudaPrototypeBuilder)
        find_package(args CONFIG REQUIRED)
        add_executable(HUHCudaPrototypeBuilder "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/CudaPrototypeBuildTool.cpp")
        target_link_libraries(HUHCudaPrototypeBuilder PRIVATE taywee::args libclang HUH::Core)
    endif ()

    get_target_property(out ${CreateOptions_TARGET} LINK_LIBRARIES)
    message(STATUS "LIBS ${out}")

    # TODO create try run to get number of files
    get_transitive_shared_library_names(${CreateOptions_TARGET} SHARED_LIB_NAMES)
    get_all_public_include_directories(${CreateOptions_TARGET} INCLUDE_LIB_PATHS)


    message(STATUS "Include: ${INCLUDE_LIB_PATHS}")

    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/Generated/Cuda)
    add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/Generated/Cuda/test.cpp DEPENDS HUHCudaPrototypeBuilder COMMAND HUHCudaPrototypeBuilder "-n" ${CreateOptions_TARGET} "-f" "$<FILTER:$<TARGET_PROPERTY:${CreateOptions_TARGET},SOURCES>,INCLUDE,\\.(cu|cuh)$>" "-o" "${CMAKE_BINARY_DIR}/Generated/Cuda" "-l" "$<JOIN:$<REMOVE_DUPLICATES:$<LIST:TRANSFORM,${SHARED_LIB_NAMES},REPLACE,^.*[/\\],>>, >" "-l" "$<REMOVE_DUPLICATES:$<LIST:TRANSFORM,${CUDA_cudadevrt_LIBRARY},REPLACE,^.*[/\\],>>" "-i" "$<JOIN:${INCLUDE_LIB_PATHS}, >" VERBATIM)

    add_library(TESTLIB ${CMAKE_BINARY_DIR}/Generated/Cuda/test.cpp)
    get_target_property(test_a ${CreateOptions_TARGET} STATIC_LIBRARY_OPTIONS)
    message(WARNING "HUH: ${CUDA_TOOLKIT_ROOT_DIR}")

endfunction()