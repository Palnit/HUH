set(VULKAN_RHI_INC_PATH "include/HUH/RHI/vulkan")

set(VULKAN_RHI_HEADER_FILES
        ${VULKAN_RHI_INC_PATH}/dynamic_rhi.h
)
set(VULKAN_RHI_SOURCE_FILES
        src/vulkan/dynamic_rhi.cpp
)
find_package(volk CONFIG REQUIRED)

add_library(HUH-VulkanRHI SHARED ${VULKAN_RHI_HEADER_FILES} ${VULKAN_RHI_SOURCE_FILES})
add_library(HUH::VulkanRHI ALIAS HUH-VulkanRHI)
target_compile_definitions(HUH-VulkanRHI PRIVATE HUH_COMP)

set_target_properties(HUH-VulkanRHI PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(HUH-VulkanRHI
        HUH
        HUH::Core
        HUH::RHI
        volk::volk
        volk::volk_headers
)

target_include_directories(HUH-VulkanRHI PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
