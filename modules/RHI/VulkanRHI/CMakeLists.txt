set(VULKAN_RHI_INC_PATH "include/HUH/RHI/vulkan")

set(VULKAN_RHI_HEADER_FILES
        ${VULKAN_RHI_INC_PATH}/dynamic_rhi.h
        ${VULKAN_RHI_INC_PATH}/vulkan_defines.h
        ${VULKAN_RHI_INC_PATH}/device.h
        ${VULKAN_RHI_INC_PATH}/queue.h
        ${VULKAN_RHI_INC_PATH}/swapchain.h
        ${VULKAN_RHI_INC_PATH}/shader.h
        ${VULKAN_RHI_INC_PATH}/pipeline.h
        ${VULKAN_RHI_INC_PATH}/command_buffer.h
        ${VULKAN_RHI_INC_PATH}/Types/image.h
)
set(VULKAN_RHI_SOURCE_FILES
        src/dynamic_rhi.cpp
        src/device.cpp
        src/queue.cpp
        src/swapchain.cpp
        src/shader.cpp
        src/pipeline.cpp
        src/command_buffer.cpp
        src/Types/image.cpp
)

if (TARGET HUH::VulkanLoader)
    set_property(TARGET HUH APPEND PROPERTY HUH_REQUIRED_INSTANCE_EXTENSIONS ${HUH_REQUIRED_INSTANCE_EXTENSIONS})
    set_property(TARGET HUH APPEND PROPERTY HUH_REQUIRED_INSTANCE_LAYERS ${HUH_REQUIRED_INSTANCE_LAYERS})
    set_property(TARGET HUH APPEND PROPERTY HUH_REQUIRED_DEVICE_EXTENSIONS ${HUH_REQUIRED_DEVICE_EXTENSIONS})

    file(GENERATE OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/include/HUH/RHI/vulkan/vulkan_defines.h INPUT configure_file/vulkan_defines.h.in)
    add_library(HUH-VulkanRHI SHARED ${VULKAN_RHI_HEADER_FILES} ${VULKAN_RHI_SOURCE_FILES})
    add_library(HUH::VulkanRHI ALIAS HUH-VulkanRHI)
    huh_target_create_definition(HUH-VulkanRHI)
    set_target_properties(HUH-VulkanRHI PROPERTIES LINKER_LANGUAGE CXX)

    target_link_libraries(HUH-VulkanRHI PUBLIC
            HUH
            HUH::Core
            HUH::Math
            HUH::RHI
            HUH::VulkanLoader
    )

    if (BUILD_WINDOW)
        target_link_libraries(HUH-VulkanRHI PRIVATE
                HUH::Window
        )
    endif ()
    target_include_directories(HUH-VulkanRHI PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
else ()
    message(WARNING "Vulkan Loader not built skipping")
endif ()

