#pragma once

#include <HUH/RHI/rhi_module.h>

namespace HUH::RHI {
class Pipeline {
public:
    friend class CommandBuffer;
    friend class Device;
    virtual bool Init() = 0;
    virtual void Destroy() = 0;
    virtual void AddShader(class Shader* shader) = 0;

protected:
    Pipeline() = default;
    virtual ~Pipeline() = default;
};
}// namespace HUH::RHI