#pragma once
#include <HUH/definitions.h>
#include <HUH/enum_define.h>
#include <HUH/vulkan_loader.h>
#include <format>
#include <string>

namespace HUH {

HUH_VULKANLOADER_API std::string ToString(VkResult inEnum);

HUH_VULKANLOADER_API std::string ToString(VkFormat inEnum);

HUH_VULKANLOADER_API std::string ToString(VkShaderStageFlagBits inEnum);

#if defined(VK_KHR_surface)
HUH_VULKANLOADER_API std::string ToString(VkColorSpaceKHR inEnum);
HUH_VULKANLOADER_API std::string ToString(VkPresentModeKHR inEnum);
#endif

}// namespace HUH

HUH_ENUM_FORMATER(VkResult)
HUH_ENUM_FORMATER(VkFormat)
HUH_ENUM_FORMATER(VkShaderStageFlagBits)
#if defined(VK_KHR_surface)
HUH_ENUM_FORMATER(VkColorSpaceKHR)
HUH_ENUM_FORMATER(VkPresentModeKHR)
#endif
