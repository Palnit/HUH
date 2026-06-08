#include <HUH/Cuda/memory_allocator.h>
namespace HUH::Cuda {

void* MemoryAllocator::MapRHIBuffer(HUH::RHI::Buffer* Buffer) {
    if (Buffer == nullptr) {
        return nullptr;
    }
    auto Shm = Buffer->GetSharedMemory();
    return nullptr;
}

}// namespace HUH::Cuda
