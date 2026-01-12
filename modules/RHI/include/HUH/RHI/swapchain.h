#pragma once
#include <HUH/definitions.h>

namespace HUH::RHI {
class HUH_RHI_API Swapchain {
public:
    friend class DynamicRHI;

protected:
    Swapchain() = default;
    virtual ~Swapchain() = default;
};
}// namespace HUH::RHI