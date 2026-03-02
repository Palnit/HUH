#pragma once

#include <HUH/RHI/dynamic_rhi.h>
#include <HUH/RHI/rhi_module.h>

namespace HUH::RHI {
class Device;

class Image {
public:
    friend class DynamicRHI;
    friend class Swapchain;

    struct Intializer {
        HUH::RHI::Device* Device = nullptr;
        HUH::RHI::Format Format = Format::UNKNOWN;
        Uint32 MipLevels = 0;
    };
    virtual bool Init(Intializer&& init) = 0;
    virtual void Destroy() = 0;

protected:
    Image(bool created) : m_created(created) {}
    virtual ~Image() = default;
    bool m_created = false;
};
}// namespace HUH::RHI