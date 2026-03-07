#pragma once
#include <HUH/definitions.h>
#include <HUH/vulkan_loader.h>
#include <string>
#include <HUH/enum_helper.h>
#include <format>

namespace HUH {

HUH_VULKANLOADER_API std::string ToString(VkResult inEnum);

HUH_VULKANLOADER_API std::string ToString(VkFormat inEnum);

HUH_VULKANLOADER_API std::string ToString(VkShaderStageFlagBits inEnum);

#if defined(VK_KHR_surface)
HUH_VULKANLOADER_API std::string ToString(VkColorSpaceKHR inEnum);
HUH_VULKANLOADER_API std::string ToString(VkPresentModeKHR inEnum);
#endif

}// namespace HUH

ENUM_FORMATER(VkResult)
ENUM_FORMATER(VkFormat)
ENUM_FORMATER(VkShaderStageFlagBits)
#if defined(VK_KHR_surface)
ENUM_FORMATER(VkColorSpaceKHR)
ENUM_FORMATER(VkPresentModeKHR)
#endif
