#pragma once
#include "HUH/types.h"
#include <HUH/definitions.h>
#include <HUH/RHI/dynamic_rhi.h>

namespace HUH {
class Window;
namespace RHI {

class Fence;
class Image;
class Queue;

class HUH_RHI_API Swapchain {
public:
    enum class PresentMode { Immediate, VSync };

    friend class Device;

    virtual bool Init(HUH::RHI::Format format, PresentMode present_mode, Uint32 MinImageCount) = 0;
    virtual void Destroy();

    Image* operator[](const size_t index) const { return m_images[index]; }
    HUH_NODISCARD size_t size() const { return m_images.size(); }
    virtual Image* NextImage(Fence* fence) = 0;
    virtual Image* NextImage(Fence* fence, Uint64 timeout) = 0;
    virtual void Present(Queue* queue, Fence* wait) = 0;
    HUH_NODISCARD Format GetFormat() const { return m_format; }

protected:
    explicit Swapchain(Window* window) : m_windowParent(window) {}
    virtual ~Swapchain() = default;
    Window* m_windowParent;
    std::vector<Image*> m_images;
    Format m_format = Format::UNKNOWN;
};
}// namespace RHI

}// namespace HUH