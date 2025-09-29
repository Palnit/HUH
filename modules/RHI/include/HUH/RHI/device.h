#pragma once

#include <HUH/definitions.h>

namespace HUH::RHI {
class HUH_API Device {
public:
    friend class DynamicRHI;
    enum class Type { Dedicated, Integrated };

    virtual Device::Type GetType() = 0;

protected:
    Device() = default;
    virtual ~Device() = default;
};
}// namespace HUH::RHI