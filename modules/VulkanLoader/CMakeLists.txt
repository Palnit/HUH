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
    add_library(HUH-VulkanLoader SHARED
            include/HUH/vulkan_loader.h
            include/HUH/VulkanHelpers/string_converters.h
            src/vulkan_loader.cpp
            src/string_converters.cpp
    )
    add_library(HUH::VulkanLoader ALIAS HUH-VulkanLoader)
    target_compile_definitions(HUH-VulkanLoader PRIVATE HUH_COMP)
    huh_target_compile_options(HUH-VulkanLoader PRIVATE GCClang -fPIC)
    huh_target_create_definition(HUH-VulkanLoader)

    set_target_properties(HUH-VulkanLoader PROPERTIES LINKER_LANGUAGE CXX)

    target_link_libraries(HUH-VulkanLoader PUBLIC
            HUH
            HUH::Core
    )
    target_precompile_headers(HUH-VulkanLoader PUBLIC include/HUH/vulkan_loader.h)

    target_include_directories(HUH-VulkanLoader PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" ${VULKAN_INC})
else ()
    message(WARNING "No vulkan headers found skipping Vulkan Loader")
endif ()
