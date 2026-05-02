#pragma once
#include "HUH/enum_define.h"

namespace HUH::RHI {
enum class RenderApi { Vulkan, DX12, Unknown };

enum class Format {
    Unknown,
    R8G8B8A8_UNORM,
    R8G8B8A8_SRGB,
    B8G8R8A8_UNORM,
    B8G8R8A8_SRGB,
};

// TODO what to do with this ??? dx12 no equivalent ??
enum class AccessType {
    Unknown = 0,
    IndexRead = 1 << 0,
    VertexAttributeRead = 1 << 1,
    UniformRead = 1 << 2,
    InputRead = 1 << 3,
    ShaderRead = 1 << 4,
    ShaderWrite = 1 << 5,
    ColorRead = 1 << 6,
    ColorWrite = 1 << 7,
    DepthStencilRead = 1 << 8,
    DepthStencilWrite = 1 << 9,
    TransferRead = 1 << 10,
    TransferWrite = 1 << 11,
    HostRead = 1 << 12,
    HostWrite = 1 << 13,
    MemoryRead = 1 << 14,
    MemoryWrite = 1 << 15,
};

enum class SyncType {
    GpuToGpu,
    GpuToCpu,
};
}// namespace HUH::RHI

HUH_ENUM_CLASS_EQUAL_OPERATOR(HUH::RHI::AccessType)
HUH_ENUM_BIT_OPERATORS(HUH::RHI::AccessType)
// TODO FORMATER
// HUH_ENUM_FORMATER(HUH::RHI::AccessType)
