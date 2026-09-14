#pragma once

#include <HUH/RHI/Types/fence.h>
#include <driver_types.h>
#include <HUH/definitions.h>
#include <HUH/lib_defines.h>

namespace HUH::Cuda {
class HUH_CUDA_API Stream {
public:
    friend class Function;
    Stream();
    void WaitFence(HUH::RHI::Fence* fence);
    void SignalFence(HUH::RHI::Fence* fence);
    ~Stream();

protected:
    cudaStream_t m_stream = nullptr;
    cudaExternalSemaphore_t GetSemaphore(HUH::RHI::Buffer::SharedMemoryInfo::PlatformHandle);
    struct SemaphoreInfo {
        HUH::RHI::Buffer::SharedMemoryInfo::PlatformHandle Handle;
        cudaExternalSemaphore_t CudaSemaphore;
    };
    std::vector<SemaphoreInfo> m_fenceHandlers;
};
}// namespace HUH::Cuda
