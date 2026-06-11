#include <HUH/Cuda/device.h>

#include "HUH/Cuda/definitions.h"

#include <cuda_runtime_api.h>

namespace HUH::Cuda {

Device::~Device() = default;

void Device::ActivateDevice() const {
    HUH_CUDA_ERR(cudaSetDevice(m_deviceId)) {
        HUH_ELOG(LogCuda, "Error Setting Cuda Device {}", err)
    }
}

std::vector<Device*> Device::EnumerateDevices() {
    int count = 0;

    HUH_CUDA_ERR(cudaGetDeviceCount(&count)) {
        HUH_ELOG(LogCuda, "Error Getting Device Count {}", err)
        return {};
    }
    if (count == 0) {
        HUH_WLOG(LogCuda, "No supported cuda devices ")
        return {};
    }

    std::vector<Device*> devices;
    for (int i = 0; i < count; i++) {
        devices.push_back(new Device(i));
    }

    return devices;
}

Device* Device::CreateFromRHI(HUH::RHI::Device* device) {
    int count = 0;

    HUH_CUDA_ERR(cudaGetDeviceCount(&count)) {
        HUH_ELOG(LogCuda, "Error Getting Device Count {}", err)
        return nullptr;
    }

    if (count == 0) {
        HUH_ELOG(LogCuda, "No supported cuda devices couldn't create cuda device from RHI device")
        return nullptr;
    }

    for (int i = 0; i < count; i++) {
        cudaDeviceProp prop{};
        cudaGetDeviceProperties(&prop, i);

        int ret = memcmp(&prop.uuid, device->Information.DeviceUUID, device->Information.DeviceUUIDSize);
        if (ret == 0) {
            return new Device(i);
        }
    }

    return nullptr;
}

Device::Device(int id) : m_deviceId(id) {
    if (id < 0) {
        HUH_ELOG(LogCuda, "Error Creating Device Invalid ID {}", id)
        return;
    }
    cudaDeviceProp prop{};
    cudaGetDeviceProperties(&prop, m_deviceId);
    Properties.MaxGrid = {prop.maxGridSize[0], prop.maxGridSize[1], prop.maxGridSize[2]};
    Properties.MaxBlock = {prop.maxThreadsDim[0], prop.maxThreadsDim[1], prop.maxThreadsDim[2]};
    Properties.MaxThread = prop.maxThreadsPerBlock;
    Properties.Major = prop.major;
    Properties.Minor = prop.minor;
    Properties.Name = prop.name;
#ifdef HUH_DEBUG
    HUH_LOG(LogCuda, Logging::DebugLog, "Cuda Device Created Named: {}", Properties.Name)
    HUH_LOG(LogCuda, Logging::DebugLog, "\t MaxGrid: {}", Properties.MaxGrid)
    HUH_LOG(LogCuda, Logging::DebugLog, "\t MaxBlock: {}", Properties.MaxBlock)
    HUH_LOG(LogCuda, Logging::DebugLog, "\t MaxThread: {}", Properties.MaxThread)
#endif
}

}// namespace HUH::Cuda
