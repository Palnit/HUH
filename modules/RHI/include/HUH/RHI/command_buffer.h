#pragma once
#include <HUH/Math/fwd.h>

namespace HUH::RHI {
class CommandBuffer {
public:
    friend class DynamicRHI;
    virtual bool Begin() = 0;
    virtual void End() = 0;
    virtual void AddRenderTarget(class Image* renderTarget) = 0;
    void SetViewPort(Vector2i viewPort) { m_viewPort = viewPort; };
    void SetScissor(Vector2i Scissor) { m_scissor = Scissor; };
    virtual bool Submit() = 0;
    virtual void Reset() = 0;
    virtual void Init(class Device* device, class Queue* queue) = 0;
    virtual void Destroy() = 0;

protected:
    CommandBuffer() = default;
    virtual ~CommandBuffer() = default;
    Vector2i m_viewPort{0, 0};
    Vector2i m_scissor{0, 0};
};
}// namespace HUH::RHI