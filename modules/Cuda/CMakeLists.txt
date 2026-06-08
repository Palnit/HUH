find_package(CUDAToolkit)

if (CUDAToolkit_FOUND)
    set_property(TARGET HUH APPEND PROPERTY HUH_REQUIRED_INSTANCE_EXTENSIONS VK_KHR_external_memory_capabilities VK_KHR_external_semaphore_capabilities)
    set_property(TARGET HUH APPEND PROPERTY HUH_REQUIRED_DEVICE_EXTENSIONS VK_KHR_external_memory VK_KHR_external_semaphore)
    if (HUH_WIN)
        set_property(TARGET HUH APPEND PROPERTY HUH_REQUIRED_DEVICE_EXTENSIONS VK_KHR_external_memory_win32 VK_KHR_external_semaphore_win32)
    elseif (HUH_LINUX)
        set_property(TARGET HUH APPEND PROPERTY HUH_REQUIRED_DEVICE_EXTENSIONS VK_KHR_external_memory_fd VK_KHR_external_semaphore_fd)
    endif ()

    set(INC_PATH "include/HUH/Cuda")
    set(HEADER_FILES
            ${INC_PATH}/definitions.h
            ${INC_PATH}/device.h
            ${INC_PATH}/memory_allocator.h
            ${INC_PATH}/module.h
            ${INC_PATH}/stream.h
    )
    set(SOURCE_FILES
            src/definitions.cpp
            src/device.cpp
            src/memory_allocator.cpp
            src/module.cpp
            src/stream.cpp
    )

    add_library(HUH-Cuda SHARED ${HEADER_FILES} ${SOURCE_FILES})
    add_library(HUH::Cuda ALIAS HUH-Cuda)
    huh_target_create_definition(HUH-Cuda)
    target_link_libraries(HUH-Cuda PUBLIC
            HUH::Core
            HUH::Math
            CUDA::cudart
            CUDA::cuda_driver
    )

    if (BUILD_RHI)
        target_link_libraries(HUH-Cuda PUBLIC HUH::RHI)
    endif ()


    target_include_directories(HUH-Cuda PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
endif ()