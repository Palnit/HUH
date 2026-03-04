#pragma once

#include <HUH/RHI/rhi_module.h>

namespace HUH::RHI {
class Pipeline {
public:
    virtual bool Init() = 0;
    virtual void Destroy() = 0;

protected:
    Pipeline() = default;
    virtual ~Pipeline() = default;
};
}// namespace HUH::RHI