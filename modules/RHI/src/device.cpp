#include <HUH/RHI/device.h>

#include "HUH/RHI/Types/barrier.h"
#include "HUH/RHI/memory_allocator.h"
#include "HUH/RHI/render_pass.h"

#include <HUH/RHI/Types/fence.h>
#include <HUH/RHI/command_pool.h>
#include <HUH/RHI/pipeline.h>
#include <HUH/RHI/queue.h>
#include <HUH/RHI/shader.h>
#include <HUH/RHI/swapchain.h>

namespace HUH {
namespace RHI {
void Device::Destroy() {
    for (MemoryAllocator* memoryAllocator : m_createdMemoryAllocators) {
        memoryAllocator->Destroy();
        delete memoryAllocator;
    }
    for (auto fence : m_createdFences) {
        delete fence;
    }
    for (CommandPool* cmd : m_createdCommandBuffers) {
        cmd->Destroy();
        delete cmd;
    }
    for (Pipeline* pipeline : m_createdPipelines) {
        pipeline->Destroy();
        delete pipeline;
    }
    for (RenderPass* renderPass : m_createdRenderPasses) {
        renderPass->Destroy();
        delete renderPass;
    }
    for (Shader* shader : m_createdShaders) {
        shader->Destroy();
        delete shader;
    }
    for (Swapchain* surface : m_createdSwapchains) {
        surface->Destroy();
        delete surface;
    }
    for (auto queue : m_queues) {
        delete queue;
    }
}
Queue* Device::GetQueue(const size_t index) const {
    return m_queues[index];
}

Device::~Device() {
}
}// namespace RHI

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