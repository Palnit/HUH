#pragma once
#include <HUH/Math/vector.h>
#include <vector>

namespace HUH::RHI {
class Fence;
class HUH_RHI_API CommandPool {
public:
    class CommandBuffer {
    public:
        friend class Device;
        friend class CommandPool;
        virtual bool Begin() = 0;
        virtual void End() = 0;
        virtual void AddRenderTarget(class Image* renderTarget) = 0;
        void SetViewPort(Vector2u32 viewPort) { m_viewPort = viewPort; };
        void SetScissor(Vector2u32 Scissor) { m_scissor = Scissor; };
        virtual bool Submit(Fence* wait, Fence* signal, Fence* waitSignal) = 0;
        virtual void Reset() = 0;

        Vector2u32 m_viewPort{0, 0};
        Vector2u32 m_scissor{0, 0};

    protected:
        CommandBuffer() = default;
        virtual ~CommandBuffer() = default;
    };

    friend class Device;
    virtual bool Init(Uint32 bufferCount, class Queue* queue) = 0;
    virtual void Destroy();
    CommandBuffer* operator[](const size_t index) const { return m_commandBuffers[index]; };

protected:
    CommandPool() = default;
    virtual ~CommandPool() = default;
    std::vector<CommandBuffer*> m_commandBuffers;
};
}// namespace HUH::RHI