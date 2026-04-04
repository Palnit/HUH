#pragma once

#include <HUH/RHI/fwd.h>
#include <HUH/enum_helper.h>

namespace HUH {
namespace RHI {
class Buffer {
public:
    friend class Device;
    friend class MemoryAllocator;
    enum Type { VERTEX, INDEX, UNIFORM, SRC, DST };
    virtual void Destroy() = 0;

protected:
    Buffer(Uint64 size);
    virtual ~Buffer() = default;
    MemoryAllocator* m_allocator = nullptr;
    Uint64 m_size = 0;
};

}// namespace RHI
std::string ToString(RHI::Buffer::Type type);
}// namespace HUH

HUH_ENUM_BIT_OPERATORS(HUH::RHI::Buffer::Type)
HUH_ENUM_FORMATER(HUH::RHI::Buffer::Type)
