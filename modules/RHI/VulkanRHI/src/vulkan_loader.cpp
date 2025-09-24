#include <HUH/RHI/vulkan/vulkan_loader.h>

namespace HUH {
bool LoadVulkan() {
#ifdef HUH_WIN
    if (!s_vulkan_lib.Load("vulkan-1.dll")) { return false; }
#elif defined(HUH_LINUX)
    if (!s_vulkan_lib.Load("libvulkan.so")) {
        if (!s_vulkan_lib.Load("libvulkan.so.1")) { return false; }
    }
#endif
    LoadVulkanFunc(vkGetInstanceProcAddr);
    LoadVulkanFunc(vkCreateInstance);
    return true;
}
bool LoadVulkanInstance() { return false; }
}// namespace HUH
