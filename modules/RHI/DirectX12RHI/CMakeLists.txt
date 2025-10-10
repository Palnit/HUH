set(DX12_RHI_INC_PATH "include/HUH/RHI/DX12")

set(DX12_RHI_HEADER_FILES
        ${DX12_RHI_INC_PATH}/dynamic_rhi.h
        ${DX12_RHI_INC_PATH}/device.h
        ${DX12_RHI_INC_PATH}/dx12_defines.h
)
set(DX12_RHI_SOURCE_FILES
        src/dynamic_rhi.cpp
        src/device.cpp
)

if (HUH_WIN)
    add_library(HUH-DirectX12RHI SHARED ${DX12_RHI_HEADER_FILES} ${DX12_RHI_SOURCE_FILES})
    add_library(HUH::DirectX12RHI ALIAS HUH-DirectX12RHI)
    target_compile_definitions(HUH-DirectX12RHI PRIVATE HUH_COMP)

    set_target_properties(HUH-DirectX12RHI PROPERTIES LINKER_LANGUAGE CXX)

    target_link_libraries(HUH-DirectX12RHI
            HUH
            HUH::Core
            HUH::RHI
            "d3d12.lib"
            "dxgi.lib"
    )
    target_include_directories(HUH-DirectX12RHI PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
else ()
    message(INFO "DirectX 12 RHI will not be built only supported on windows")
endif ()
