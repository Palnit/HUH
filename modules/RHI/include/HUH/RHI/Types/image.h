#pragma once

#include <HUH/RHI/dynamic_rhi.h>
#include <HUH/RHI/fwd.h>
#include <HUH/enum_define.h>

#include <HUH/FileHandling/Image/image.h>

#include <HUH/Math/vector.h>

#include <HUH/types.h>

namespace HUH::RHI {

class Image {
public:
    friend class DynamicRHI;
    friend class Swapchain;
    friend class Pipeline;

    enum Type {
        Unknown = 0,
        Color = 1 << 0,
        DeptStencil = 1 << 1,
        Transient = 1 << 2,
        Sampled = 1 << 3,
        Storage = 1 << 4,
        SRC = 1 << 5,
        DST = 1 << 6,
    };

    struct Initializer {
        HUH::RHI::Format Format = Format::Unknown;
        Uint32 MipLevels = 0;
        HUH::Vector2u32 Size = {0, 0};
    };

    virtual bool Init(Initializer&& init) = 0;
    virtual void Destroy() = 0;
    // TODO manual uploading of image data without copying from normal buffer
    HUH_NODISCARD HUH::Vector2u32 GetSize() const { return m_size; }

protected:
    explicit Image(bool created) : m_created(created) {}
    virtual ~Image() = default;
    bool m_created = false;
    HUH::Vector2u32 m_size;
    MemoryAllocator* m_allocator = nullptr;
};
}// namespace HUH::RHI

HUH_ENUM_BIT_OPERATORS(HUH::RHI::Image::Type)
