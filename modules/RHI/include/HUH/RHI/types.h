#pragma once

namespace HUH::RHI {
enum class RenderApi { Vulkan, DX12, Unknown };

enum class SyncType {
    GpuToGpu,
    GpuToCpu,
};
}// namespace HUH::RHI
