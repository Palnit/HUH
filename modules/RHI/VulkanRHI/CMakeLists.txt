set(VULKAN_RHI_INC_PATH "include/HUH/RHI/vulkan")

set(VULKAN_RHI_HEADER_FILES
        ${VULKAN_RHI_INC_PATH}/dynamic_rhi.h
        ${VULKAN_RHI_INC_PATH}/vulkan_loader.h
)
set(VULKAN_RHI_SOURCE_FILES
        src/dynamic_rhi.cpp
        src/vulkan_loader.cpp
)

if (VULKAN_HEADER_DIR)
    set(VULKAN_INC ${VULKAN_HEADER_DIR}/include)
else ()
    find_package(Vulkan)
    if (Vulkan_INCLUDE_DIRS)
        set(VULKAN_INC "${Vulkan_INCLUDE_DIRS}")
    elseif (DEFINED ENV{VULKAN_SDK})
        set(VULKAN_INC "$ENV{VULKAN_SDK}/include")
    endif ()
endif ()

if (VULKAN_INC)
    message(STATUS "Vulkan headers found location: ${VULKAN_INC}")
    add_library(HUH-VulkanRHI SHARED ${VULKAN_RHI_HEADER_FILES} ${VULKAN_RHI_SOURCE_FILES})
    add_library(HUH::VulkanRHI ALIAS HUH-VulkanRHI)
    target_compile_definitions(HUH-VulkanRHI PRIVATE HUH_COMP)

    set_target_properties(HUH-VulkanRHI PROPERTIES LINKER_LANGUAGE CXX)

    target_link_libraries(HUH-VulkanRHI
            HUH
            HUH::Core
            HUH::RHI
    )

    target_include_directories(HUH-VulkanRHI PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" ${VULKAN_INC})
else ()
    message(WARNING "No vulkan headers found skipping Vulkan RHI")
endif ()

