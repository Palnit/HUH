#pragma once

#ifdef HUH_USE_RHI
#include <HUH/RHI/device.h>
#endif

namespace HUH::Cuda {
class Device;

class Device {
public:
    struct Properties {
        HUH::Uint64 MaxThread;
        HUH::Vector3i MaxBlock;
        HUH::Vector3i MaxGrid;
        std::string Name;
    };

    Properties Properties;

    void Destroy() const { delete this; }
    void ActivateDevice() const;
    static std::vector<Device*> EnumerateDevices();
    HUH_NODISCARD bool IsValid() const { return m_deviceId != 0; }

#ifdef HUH_USE_RHI
    static Device* CreateFromRHI(HUH::RHI::Device* device);
#endif

protected:
    explicit Device(int id);
    ~Device();

    int m_deviceId;
};

}// namespace HUH::Cuda