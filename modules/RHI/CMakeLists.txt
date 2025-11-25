set(RHI_INC_PATH "include/HUH/RHI")


set(HEADER_FILES
        ${RHI_INC_PATH}/dynamic_rhi.h
        ${RHI_INC_PATH}/types.h
        ${RHI_INC_PATH}/device.h
        ${RHI_INC_PATH}/rhi_module.h
        ${RHI_INC_PATH}/queue.h
        ${RHI_INC_PATH}/surface.h
)
set(SOURCE_FILES
        src/dynamic_rhi.cpp
        src/device.cpp
        src/queue.cpp
        src/surface.cpp
)

add_library(HUH-RHI SHARED ${HEADER_FILES} ${SOURCE_FILES})
add_library(HUH::RHI ALIAS HUH-RHI)
huh_target_create_definition(HUH-RHI)

set_target_properties(HUH-RHI PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(HUH-RHI PUBLIC
        HUH
        HUH::Core
)


target_include_directories(HUH-RHI PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_subdirectory(VulkanRHI)
add_subdirectory(DirectX12RHI)
