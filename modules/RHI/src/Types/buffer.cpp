#include <HUH/RHI/Types/buffer.h>

#include "HUH/RHI/rhi_module.h"
#include "HUH/logging.h"

#include <HUH/enum_helper.h>

namespace HUH {
namespace RHI {
Buffer::Buffer(Uint64 size) : m_size(size) {
}
}// namespace RHI

std::string ToString(RHI::Buffer::Type type) {
    std::string result = "";
    if (HUH::CheckFlag(type, HUH::RHI::Buffer::VERTEX)) {
        result += "VERTEX | ";
    }
    if (HUH::CheckFlag(type, HUH::RHI::Buffer::INDEX)) {
        result += "INDEX | ";
    }
    if (HUH::CheckFlag(type, HUH::RHI::Buffer::UNIFORM)) {
        result += "UNIFORM | ";
    }
    if (HUH::CheckFlag(type, HUH::RHI::Buffer::SRC)) {
        result += "SRC | ";
    }
    if (HUH::CheckFlag(type, HUH::RHI::Buffer::DST)) {
        result += "DST | ";
    }
    result = result.substr(0, result.size() - 3);
    return result;
}
}// namespace HUH