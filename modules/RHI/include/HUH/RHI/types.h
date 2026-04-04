#pragma once

namespace HUH::RHI {
enum class RenderApi { Vulkan, DX12, Unknown };

enum class Format {
    UNKNOWN,
    R8G8B8A8_UNORM,
    R8G8B8A8_SRGB,
    B8G8R8A8_UNORM,
    B8G8R8A8_SRGB,
};

enum class SyncType {
    GpuToGpu,
    GpuToCpu,
};
}// namespace HUH::RHI
