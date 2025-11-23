#include <HUH/RHI/device.h>
namespace HUH::RHI {

Queue* Device::GetQueue(const size_t index) const {
    return m_queues[index];
}

Device::~Device() {
    for (auto queue : m_queues) {
        delete queue;
    }
}
}// namespace HUH::RHI