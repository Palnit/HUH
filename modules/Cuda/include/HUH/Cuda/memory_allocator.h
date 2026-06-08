#pragma once

#include <HUH/RHI/device.h>
#include <driver_types.h>
#include <map>

namespace HUH::Cuda {

class MemoryAllocator {
public:
    void* MapRHIBuffer(HUH::RHI::Buffer* Buffer);

protected:
    std::map<HUH::RHI::Buffer::SharedMemoryInfo::PlatformHandle, cudaExternalMemory_t> m_MappedMemories;
};

}// namespace HUH::Cuda
