#pragma once

#include <HUH/RHI/dynamic_rhi.h>
#include <HUH/types.h>
#include <HUH/Math/vector.h>
#include <HUH/RHI/fwd.h>

namespace HUH::RHI {

class Image {
public:
    friend class DynamicRHI;
    friend class Swapchain;

    struct Initializer {
        HUH::RHI::Device* Device = nullptr;
        HUH::RHI::Format Format = Format::Unknown;
        Uint32 MipLevels = 0;
        HUH::Vector2u32 Size;
    };
    virtual bool Init(Initializer&& init) = 0;
    virtual void Destroy() = 0;

protected:
    Image(bool created) : m_created(created) {}
    virtual ~Image() = default;
    bool m_created = false;
    HUH::Vector2u32 m_size;
};
}// namespace HUH::RHI