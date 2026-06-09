#include <HUH/Cuda/memory_allocator.h>

#include "HUH/Cuda/definitions.h"

#include <cuda_runtime_api.h>
namespace HUH::Cuda {

void* MemoryAllocator::MapRHIBuffer(HUH::RHI::Buffer* Buffer) {
    if (Buffer == nullptr) {
        return nullptr;
    }
    auto Shm = Buffer->GetSharedMemory();
    for (auto& alloc : m_allocations) {
        if (alloc.PlatformHandle == Shm.Handle) {
            return alloc.MapMemory(Shm);
        }
    }
    Allocation allocation;
    cudaExternalMemoryHandleDesc externalMemoryHandleDesc = {
#if defined(HUH_LINUX)
        .type = cudaExternalMemoryHandleTypeOpaqueFd,
        .handle = {.fd = Shm.Handle.Fd},
#elif defined(HUH_WIN)
#endif
        .size = Shm.FullSize,
    };
    HUH_CUDA_ERR(cudaImportExternalMemory(&allocation.ExternalMemory, &externalMemoryHandleDesc)) {
        HUH_ELOG(LogCuda, "Error during Mapping of external memory: {}")
        return nullptr;
    }
    allocation.PlatformHandle = Shm.Handle;
    m_allocations.push_back(allocation);
    return allocation.MapMemory(Shm);
}

void* MemoryAllocator::Allocation::MapMemory(const HUH::RHI::Buffer::SharedMemoryInfo& Shm) const {
    void* cudaPtr;
    cudaExternalMemoryBufferDesc externalMemBufferDesc = {
        .offset = Shm.Offset,
        .size = Shm.Size,
        .flags = 0,
    };
    HUH_CUDA_ERR(cudaExternalMemoryGetMappedBuffer(&cudaPtr, ExternalMemory, &externalMemBufferDesc)) {
        HUH_ELOG(LogCuda, "Couldn't mapp Buffer memory to cuda memory Error: {}", err)
        return nullptr;
    }
    return cudaPtr;
}

MemoryAllocator::~MemoryAllocator() {
    for (auto& allocation : m_allocations) {
        HUH_CUDA_ERR(cudaDestroyExternalMemory(allocation.ExternalMemory)) {
            HUH_ELOG(LogCuda, "Error while destroying External memory: {}", err)
        }
    }
}

}// namespace HUH::Cuda
