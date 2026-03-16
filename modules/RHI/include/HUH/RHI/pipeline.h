#pragma once

#include <HUH/RHI/rhi_module.h>
#include <HUH/RHI/dynamic_rhi.h>

namespace HUH::RHI {
class Pipeline {
public:
    friend class CommandPool;
    friend class Device;
    virtual bool Init(std::vector<Format> formats) = 0;
    virtual void Destroy() = 0;
    virtual void AddShader(class Shader* shader) = 0;

protected:
    Pipeline() = default;
    virtual ~Pipeline() = default;
};
}// namespace HUH::RHI