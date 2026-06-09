#pragma once

#include <HUH/RHI/fwd.h>
#include <HUH/enum_define.h>
#include <HUH/types.h>

typedef void* HANDLE;

namespace HUH {
namespace RHI {
// TODO rethink buffers to maybe be template resource data and go from there to be able to bind arrays ?
class HUH_RHI_API Buffer {
public:
    friend class Device;
    friend class MemoryAllocator;
    friend class Pipeline;
    enum Type {
        Unknown = 0,
        VERTEX = 1 << 0,
        INDEX = 1 << 1,
        UNIFORM = 1 << 2,
        SRC = 1 << 3,
        DST = 1 << 4,
    };
    virtual void Destroy() = 0;
    virtual void MapData() = 0;
    virtual void CopyData(void* data) = 0;
    virtual void UploadData(void* data) = 0;
    virtual void UnMapData() = 0;
    HUH_NODISCARD Uint64 GetSize() const { return m_size; }
    HUH_NODISCARD void* GetMappedData() const { return m_mappedData; }

#ifdef HUH_USE_CUDA
    struct SharedMemoryInfo {
        struct PlatformHandle {
            int Fd = -1;
            HANDLE Handle = nullptr;
            bool Managed = false;
            bool operator==(const PlatformHandle& other) const { return Handle == other.Handle && Fd == other.Fd; }
            bool operator!=(const PlatformHandle& other) const { return !this->operator==(other); }
            HUH_NODISCARD bool IsValid() const { return Handle != nullptr || Fd != -1; }
        } Handle;
        Uint32 Offset = 0;
        Uint32 Size = 0;
        Uint32 FullSize = 0;
    };

    virtual SharedMemoryInfo GetSharedMemory() = 0;
#endif

protected:
    Buffer(Uint64 size);
    virtual ~Buffer() = default;
    MemoryAllocator* m_allocator = nullptr;
    Uint64 m_size = 0;
    void* m_mappedData = nullptr;
};

}// namespace RHI
std::string ToString(RHI::Buffer::Type type);
}// namespace HUH

HUH_ENUM_BIT_OPERATORS(HUH::RHI::Buffer::Type)
HUH_ENUM_FORMATER(HUH::RHI::Buffer::Type)
