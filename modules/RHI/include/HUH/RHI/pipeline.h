#pragma once

#include <HUH/RHI/rhi_module.h>

namespace HUH::RHI {
class Pipeline {
public:
    friend class DynamicRHI;
    friend class CommandBuffer;
    virtual bool Init(class Device* device) = 0;
    virtual void Destroy() = 0;
    virtual void AddShader(class Shader* shader) = 0;

protected:
    Pipeline() = default;
    virtual ~Pipeline() = default;
};
}// namespace HUH::RHI