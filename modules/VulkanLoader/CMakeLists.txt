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
    add_library(HUH-VulkanLoader STATIC
            include/HUH/vulkan_loader.h
            include/HUH/VulkanHelpers/string_converters.h
            src/vulkan_loader.cpp
            src/string_converters.cpp
    )
    add_library(HUH::VulkanLoader ALIAS HUH-VulkanLoader)
    target_compile_definitions(HUH-VulkanLoader PRIVATE HUH_COMP)

    set_target_properties(HUH-VulkanLoader PROPERTIES LINKER_LANGUAGE CXX)

    target_link_libraries(HUH-VulkanLoader
            HUH
            HUH::Core
    )

    target_include_directories(HUH-VulkanLoader PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" ${VULKAN_INC})
else ()
    message(WARNING "No vulkan headers found skipping Vulkan Loader")
endif ()
