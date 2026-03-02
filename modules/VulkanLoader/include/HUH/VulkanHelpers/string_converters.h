#pragma once
#include <HUH/definitions.h>
#include <HUH/vulkan_loader.h>
#include <string>
#include <HUH/enum_helper.h>

namespace HUH {

template<>
HUH_VULKANLOADER_API std::string ToString(VkResult inEnum);

template<>
HUH_VULKANLOADER_API std::string ToString(VkFormat inEnum);

template<>
HUH_VULKANLOADER_API std::string ToString(VkShaderStageFlagBits inEnum);

#if defined(VK_KHR_surface)
template<>
HUH_VULKANLOADER_API std::string ToString(VkColorSpaceKHR inEnum);
template<>
HUH_VULKANLOADER_API std::string ToString(VkPresentModeKHR inEnum);
#endif

}// namespace HUH