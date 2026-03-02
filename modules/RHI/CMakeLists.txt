set(RHI_INC_PATH "include/HUH/RHI")


set(HEADER_FILES
        ${RHI_INC_PATH}/dynamic_rhi.h
        ${RHI_INC_PATH}/types.h
        ${RHI_INC_PATH}/device.h
        ${RHI_INC_PATH}/rhi_module.h
        ${RHI_INC_PATH}/queue.h
        ${RHI_INC_PATH}/swapchain.h
        ${RHI_INC_PATH}/shader.h
        ${RHI_INC_PATH}/pipeline.h
        ${RHI_INC_PATH}/Types/image.h
)
set(SOURCE_FILES
        src/dynamic_rhi.cpp
        src/device.cpp
        src/queue.cpp
        src/swapchain.cpp
        src/shader.cpp
        src/pipeline.cpp
        src/Types/image.cpp
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
