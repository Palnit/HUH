#pragma once

#include <HUH/RHI/fwd.h>
#include <HUH/RHI/types.h>
#include <HUH/enum_define.h>
#include <map>

namespace HUH {
namespace RHI {
class HUH_RHI_API MemoryAllocator {
public:
    friend class Device;
    enum Type {
        UNKNOWN = 0,
        Device = 1 << 0,
        Host = 1 << 1,
        TransferSrc = 1 << 2,
        TransferDst = 1 << 3,
    };
    virtual bool Allocate(Buffer* buffer, Type type) = 0;
    virtual bool Allocate(Image* buffer, Type type) = 0;
    virtual bool Free(Buffer* buffer) = 0;
    virtual bool Free(Image* buffer) = 0;
    virtual void Destroy() = 0;

protected:
    MemoryAllocator() = default;
    virtual ~MemoryAllocator() = default;
};
}// namespace RHI

std::string HUH_RHI_API ToString(RHI::MemoryAllocator::Type);

}// namespace HUH

HUH_ENUM_BIT_OPERATORS(HUH::RHI::MemoryAllocator::Type);
HUH_ENUM_FORMATER(HUH::RHI::MemoryAllocator::Type)