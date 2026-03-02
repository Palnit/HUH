#include <HUH/RHI/swapchain.h>
namespace HUH::RHI {
void Swapchain::Destroy() {
    for (auto image : m_images) {
        image->Destroy();
        delete image;
    }
}
}// namespace HUH::RHI