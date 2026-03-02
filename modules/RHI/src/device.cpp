#include <HUH/RHI/device.h>
namespace HUH {
namespace RHI {
Queue* Device::GetQueue(const size_t index) const {
    return m_queues[index];
}

Device::~Device() {
    for (auto queue : m_queues) {
        delete queue;
    }
}
}// namespace RHI

template<>
std::string ToString(const RHI::Device::Type inEnum) {
    switch (inEnum) {
        case RHI::Device::Type::Dedicated:
            return "Dedicated";
        case RHI::Device::Type::Integrated:
            return "Integrated";
        case RHI::Device::Type::Cpu:
            return "Cpu";
        case RHI::Device::Type::Virtual:
            return "Virtual";
        case RHI::Device::Type::Other:
            return "Other";
        case RHI::Device::Type::Unknown:
        default:
            return "Unknown";
    }
}

template<>
std::string ToString(const RHI::Device::Vendor inEnum) {
    switch (inEnum) {
        case RHI::Device::Vendor::Nvidia:
            return "Nvidia";
        case RHI::Device::Vendor::Amd:
            return "Amd";
        case RHI::Device::Vendor::Arm:
            return "Arm";
        case RHI::Device::Vendor::Qualcomm:
            return "Qualcomm";
        case RHI::Device::Vendor::Intel:
            return "Intel";
        case RHI::Device::Vendor::Unknown:
        default:
            return "Unknown";
    }
}
}// namespace HUH