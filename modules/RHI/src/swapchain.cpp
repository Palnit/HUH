#include <HUH/RHI/swapchain.h>
#include <HUH/RHI/Types/image.h>

namespace HUH::RHI {
void Swapchain::Destroy() {
    for (auto image : m_images) {
        image->Destroy();
        delete image;
    }
    m_images.clear();
}
}// namespace HUH::RHI