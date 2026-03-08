#pragma once
#include "types.h"

#include <HUH/Math/vector.h>

namespace HUH::RHI {
template<SyncType>
class Fence;
class CommandBuffer {
public:
    friend class Device;
    virtual bool Begin() = 0;
    virtual void End() = 0;
    virtual void AddRenderTarget(class Image* renderTarget) = 0;
    void SetViewPort(Vector2u32 viewPort) { m_viewPort = viewPort; };
    void SetScissor(Vector2u32 Scissor) { m_scissor = Scissor; };
    virtual bool Submit(Fence<SyncType::GpuToGpu>* wait,
                        Fence<SyncType::GpuToGpu>* signal,
                        Fence<SyncType::GpuToCpu>* waitSignal) = 0;
    virtual void Reset() = 0;
    virtual bool Init(class Queue* queue) = 0;
    virtual void Destroy() = 0;

protected:
    CommandBuffer() = default;
    virtual ~CommandBuffer() = default;
    Vector2u32 m_viewPort{0, 0};
    Vector2u32 m_scissor{0, 0};
};
}// namespace HUH::RHI