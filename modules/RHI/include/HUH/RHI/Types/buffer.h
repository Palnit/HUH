#pragma once

#include <HUH/RHI/fwd.h>
#include <HUH/enum_define.h>
#include <HUH/types.h>

namespace HUH {
namespace RHI {
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
