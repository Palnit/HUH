#include <HUH/vulkan_loader.h>

namespace HUH {

HUH_VULKANLOADER_API HUH::DynamicLibrary g_vulkanLib;

#define CreateVulkanFuncLocal(Type) HUH_VULKANLOADER_API PFN_## Type Type;
HUH_VULKAN_ALL_FUNCS(CreateVulkanFuncLocal)
#undef CreateVulkanFuncLocal

bool LoadVulkan() {
#ifdef HUH_WIN
    if (!g_vulkanLib.Load("vulkan-1.dll")) {
        return false;
    }
#elif defined(HUH_LINUX)
    if (!g_vulkanLib.Load("libvulkan.so.1")) {
        if (!g_vulkanLib.Load("libvulkan.so")) {
            return false;
        }
    }
#endif

#define LoadVulkanNoProcFunc(Type) HUH::Type = g_vulkanLib.GetExport<std::remove_pointer<PFN_## Type>::type>(#Type);
    HUH_VULKAN_INSTANCE_FUNCS_1_0(LoadVulkanNoProcFunc)
    HUH_VULKAN_INSTANCE_FUNCS_1_1(LoadVulkanNoProcFunc)
#undef LoadVulkanNoProcFunc
    return true;
}
bool LoadVulkanInstance(VkInstance instance) {

#define LoadVulkanProcFunc(Type) HUH::Type =  reinterpret_cast<PFN_## Type>(HUH::vkGetInstanceProcAddr(instance,#Type));
    HUH_VULKAN_ALL_PROC_FUNCS(LoadVulkanProcFunc)
#undef LoadVulkanProcFunc
    return false;
}
}// namespace HUH
