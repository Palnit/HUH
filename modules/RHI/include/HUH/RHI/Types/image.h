#pragma once

#include <HUH/RHI/dynamic_rhi.h>
#include <HUH/RHI/fwd.h>

#include <HUH/FileHandling/Image/image.h>

#include <HUH/Math/vector.h>

#include <HUH/types.h>

namespace HUH::RHI {

class Image {
public:
    friend class DynamicRHI;
    friend class Swapchain;
    friend class Pipeline;

    struct Initializer {
        HUH::RHI::Device* Device = nullptr;
        HUH::RHI::Format Format = Format::Unknown;
        Uint32 MipLevels = 0;
        HUH::Vector2u32 Size = {0, 0};
    };

    virtual bool Init(Initializer&& init) = 0;
    virtual void Destroy() = 0;
    // TODO manual uploading of image data without copying from normal buffer
    HUH::Vector2u32 GetSize() const { return m_size; }

protected:
    Image(bool created) : m_created(created) {}
    virtual ~Image() = default;
    bool m_created = false;
    HUH::Vector2u32 m_size;
    MemoryAllocator* m_allocator = nullptr;
};
}// namespace HUH::RHI