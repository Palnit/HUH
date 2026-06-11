#include <HUH/Cuda/stream.h>

#include "HUH/Cuda/definitions.h"

#include <cuda_runtime_api.h>

namespace HUH::Cuda {

Stream::Stream() {
    HUH_CUDA_ERR(cudaStreamCreate(&m_stream)) {
        HUH_ELOG(LogCuda, "Error During Initialization of cuda Stream: {}", err)
    }
}

void Stream::WaitFence(HUH::RHI::Fence* fence) {
    auto semaphore = GetSemaphore(fence->GetSharedMemory().Handle);
    if (!semaphore) {
        HUH_WLOG(LogCuda, "No semaphore found for wait operation")
        return;
    }
    cudaExternalSemaphoreWaitParams params{};
    HUH_CUDA_ERR(cudaWaitExternalSemaphoresAsync(&semaphore, &params, 1, m_stream)) {
        HUH_ELOG(LogCuda, "Error while waiting semaphore: {}", err)
    }
}

void Stream::SignalFence(HUH::RHI::Fence* fence) {
    auto semaphore = GetSemaphore(fence->GetSharedMemory().Handle);
    if (!semaphore) {
        HUH_WLOG(LogCuda, "No semaphore found for Signal operation")
        return;
    }
    cudaExternalSemaphoreSignalParams params{};
    params.flags = 0;
    params.params.fence.value = 0;
    HUH_CUDA_ERR(cudaSignalExternalSemaphoresAsync(&semaphore, &params, 1, m_stream)) {
        HUH_ELOG(LogCuda, "Error while signaling semaphore: {}", err)
    }
}

Stream::~Stream() {
    if (m_stream) {
        HUH_CUDA_ERR(cudaStreamDestroy(m_stream)) {
            HUH_ELOG(LogCuda, "Error During Destruction of cuda Stream: {}", err)
        }
    }
}

cudaExternalSemaphore_t Stream::GetSemaphore(HUH::RHI::Buffer::SharedMemoryInfo::PlatformHandle Handle) {
    // for (auto& info : m_fenceHandlers) {
    //     if (info.Handle == Handle) {
    //         return info.CudaSemaphore;
    //     }
    // }
    cudaExternalSemaphoreHandleDesc desc{};
#if defined(HUH_LINUX)
    desc.type = cudaExternalSemaphoreHandleTypeOpaqueFd;
    desc.handle.fd = Handle.Fd;
#elif defined(HUH_WIN)
#endif
    SemaphoreInfo info{};
    HUH_CUDA_ERR(cudaImportExternalSemaphore(&info.CudaSemaphore, &desc)) {
        HUH_ELOG(LogCuda, "Importin external memory err: {}", err);
    }
    info.Handle = Handle;
    m_fenceHandlers.push_back(info);
    return info.CudaSemaphore;
}

}// namespace HUH::Cuda
