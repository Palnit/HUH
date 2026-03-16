#include <HUH/RHI/command_pool.h>

namespace HUH::RHI {
void CommandPool::Destroy() {
    for (auto commandBuffer : m_commandBuffers) {
        delete commandBuffer;
    }
}
}// namespace HUH::RHI