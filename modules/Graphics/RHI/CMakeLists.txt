set(RHI_INC_PATH "include/HUH/Graphics/RHI")
set(RHI_INC_PATH_VULKAN ${RHI_INC_PATH}/Vulkan)

find_package(volk CONFIG REQUIRED)

set(HEADER_FILES
        ${RHI_INC_PATH}/instance.h
        ${RHI_INC_PATH}/types.h
)
set(SOURCE_FILES
        src/instance.cpp
)

set(HEADER_FILES_VULKAN
        ${RHI_INC_PATH_VULKAN}/instance.h
)
set(SOURCE_FILES_VULKAN
        src/Vulkan/instance.cpp
)
add_library(HUH-Graphics-RHI SHARED ${HEADER_FILES} ${SOURCE_FILES} ${HEADER_FILES_VULKAN} ${SOURCE_FILES_VULKAN})
add_library(HUH::Graphics::RHI ALIAS HUH-Graphics-RHI)

set_target_properties(HUH-Graphics-RHI PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(HUH-Graphics-RHI
        HUH
        HUH::Core
        volk::volk
        volk::volk_headers
)

target_include_directories(HUH-Graphics-RHI PUBLIC "include")
