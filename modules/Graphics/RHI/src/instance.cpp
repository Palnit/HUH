#include "HUH/Graphics/RHI/instance.h"
#include <memory>
#include <mutex>
#include "HUH/Graphics/RHI/Vulkan/instance.h"

namespace HUH::RHI {
Instance* Instance::Get(RenderApi api) {
    static std::unique_ptr<Instance> singleton;
    static std::once_flag initFlag;
    std::call_once(initFlag, []() {
        singleton = std::make_unique<VulkanInstance>();
        if (!singleton->Init()) {
            std::cout << "Init failed" << std::endl;
            //TODO Make check module so that in debug this crashes and log module
        }
    });
    return singleton.get();
}
}// namespace HUH::RHI
