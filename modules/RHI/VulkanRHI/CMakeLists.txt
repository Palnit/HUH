set(VULKAN_RHI_INC_PATH "include/HUH/RHI/vulkan")

set(VULKAN_RHI_HEADER_FILES
        ${VULKAN_RHI_INC_PATH}/dynamic_rhi.h
        ${VULKAN_RHI_INC_PATH}/vulkan_defines.h
        ${VULKAN_RHI_INC_PATH}/device.h
)
set(VULKAN_RHI_SOURCE_FILES
        src/dynamic_rhi.cpp
        src/device.cpp
)

if (TARGET HUH::VulkanLoader)
    set_property(TARGET HUH APPEND PROPERTY HUH_REQUIRED_INSTANCE_EXTENSIONS ${HUH_REQUIRED_INSTANCE_EXTENSIONS})
    set_property(TARGET HUH APPEND PROPERTY HUH_REQUIRED_INSTANCE_LAYERS ${HUH_REQUIRED_INSTANCE_LAYERS})
    set_property(TARGET HUH APPEND PROPERTY HUH_REQUIRED_DEVICE_EXTENSIONS ${HUH_REQUIRED_DEVICE_EXTENSIONS})

    file(GENERATE OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/include/HUH/RHI/vulkan/vulkan_defines.h INPUT configure_file/vulkan_defines.h.in)
    add_library(HUH-VulkanRHI SHARED ${VULKAN_RHI_HEADER_FILES} ${VULKAN_RHI_SOURCE_FILES})
    add_library(HUH::VulkanRHI ALIAS HUH-VulkanRHI)
    target_compile_definitions(HUH-VulkanRHI PRIVATE HUH_COMP)

    set_target_properties(HUH-VulkanRHI PROPERTIES LINKER_LANGUAGE CXX)

    target_link_libraries(HUH-VulkanRHI
            HUH
            HUH::Core
            HUH::RHI
            HUH::VulkanLoader
    )
    target_include_directories(HUH-VulkanRHI PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" ${VULKAN_INC} "${CMAKE_CURRENT_BINARY_DIR}/include")
else ()
    message(WARNING "Vulkan Loader not built skipping")
endif ()

