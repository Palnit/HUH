#pragma once
#include <HUH/Math/fwd.h>

namespace HUH::RHI {
class CommandBuffer {
public:
    friend class DynamicRHI;
    virtual bool Begin() = 0;
    virtual void End() = 0;
    virtual void AddRenderTarget(class Image* renderTarget) = 0;
    virtual void SetViewPort(Vector2i) = 0;
    virtual void Init() = 0;
    virtual void Destroy() = 0;

protected:
    CommandBuffer() = default;
    virtual ~CommandBuffer() = default;
};
}// namespace HUH::RHI