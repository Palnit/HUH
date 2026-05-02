#include <HUH/RHI/memory_allocator.h>
#include <HUH/enum_helper.h>

namespace HUH {
namespace RHI {}
std::string ToString(RHI::MemoryAllocator::Type type) {
    std::string result = "";
    if (HUH::CheckFlag(type, RHI::MemoryAllocator::Type::Device)) {
        result += "Device | ";
    }
    if (HUH::CheckFlag(type, RHI::MemoryAllocator::Type::Host)) {
        result += "Host | ";
    }
    result = result.substr(0, result.size() - 3);
    return result;
}
}// namespace HUH
