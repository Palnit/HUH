function(huh_create_prototype)
    #    set(single_args TARGET)
    #    cmake_parse_arguments(PARSE_ARGV 0 CreateOptions "" "${single_args}" "")
    #    if (NOT DEFINED CreateOptions_TARGET)
    #        message(FATAL_ERROR "huh_create_prototype needs a cuda TARGET")
    #    endif ()
    #    if (NOT TARGET HUHCudaPrototypeBuilder)
    #        add_executable(HUHCudaPrototypeBuilder "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/CudaPrototypeBuildTool.cpp")
    #    endif ()
    #
    #    get_target_property(out ${CreateOptions_TARGET} LINK_LIBRARIES)
    #    message(STATUS "LIBS ${out}")
    #
    #    # TODO create try run to get number of files
    #
    #    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/Generated/Cuda)
    #    add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/Generated/Cuda/test.cpp DEPENDS HUHCudaPrototypeBuilder COMMAND HUHCudaPrototypeBuilder ${CreateOptions_TARGET} "test test" VERBATIM)
    #
    #    add_library(TESTLIB ${CMAKE_BINARY_DIR}/Generated/Cuda/test.cpp)
    #
    #    get_target_property(test_a ${CreateOptions_TARGET} STATIC_LIBRARY_OPTIONS)
    #    message(WARNING "HUH: ${CUDA_TOOLKIT_ROOT_DIR}")
    #
endfunction()