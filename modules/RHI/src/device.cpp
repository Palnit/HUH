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

std::string ToString(RHI::Device::Type type) {
    switch (type) {
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

std::string ToString(RHI::Device::Vendor vendor) {
    switch (vendor) {
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