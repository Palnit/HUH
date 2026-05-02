#pragma once
#include <HUH/Math/vector.h>
#include <HUH/RHI/fwd.h>
#include <vector>

namespace HUH::RHI {
class HUH_RHI_API CommandPool {
public:
    class CommandBuffer {
    public:
        friend class Device;
        friend class CommandPool;
        virtual bool Begin() = 0;
        virtual void End() = 0;
        virtual void BeginRendering(RenderPass* renderPass, class Image* renderTarget) = 0;
        virtual void EndRendering() = 0;
        void SetViewPort(const Vector2u32& viewPort) { m_viewPort = viewPort; };
        void SetScissor(const Vector2u32& Scissor) { m_scissor = Scissor; };
        void SetClearColor(const Vector4f& clearColor) { m_clearColor = clearColor; };
        virtual void BindVertexBuffer(class Buffer* buffer) = 0;
        virtual void BindIndexBuffer(Buffer* buffer) = 0;
        virtual void Draw(Uint32 vertexCount, Uint32 instanceCount) = 0;
        virtual void DrawIndexed(Uint32 indexCount, Uint32 instanceCount) = 0;
        virtual void Reset() = 0;
        virtual void BindPipeline(class Pipeline* pipeline) = 0;
        virtual void CopyBuffer(Buffer* srcBuffer, Buffer* dstBuffer) = 0;

        Vector2u32 m_viewPort{0, 0};
        Vector2u32 m_scissor{0, 0};
        Vector4f m_clearColor{0.0f, 0.0f, 0.0f, 1.0f};

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