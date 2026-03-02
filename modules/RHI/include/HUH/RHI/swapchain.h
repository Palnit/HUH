#pragma once
#include "Types/image.h"
#include <HUH/definitions.h>
#include <HUH/RHI/dynamic_rhi.h>

namespace HUH {
class Window;
namespace RHI {
class HUH_RHI_API Swapchain {
public:
    enum class PresentMode { Immediate, VSync };

    friend class DynamicRHI;
    virtual bool Init(class Device* device,
                      HUH::RHI::Format format,
                      PresentMode present_mode,
                      Uint32 MinImageCount) = 0;
    virtual void Destroy();

    Image* operator[](const size_t index) const { return m_images[index]; }
    size_t size() const { return m_images.size(); }

protected:
    explicit Swapchain(Window* window) : m_windowParent(window) {}
    virtual ~Swapchain() = default;
    Window* m_windowParent;
    std::vector<Image*> m_images;
};
}// namespace RHI
}// namespace HUH