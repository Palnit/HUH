#pragma once
#include <HUH/definitions.h>
#include <string>
#include <HUH/vulkan_loader.h>

namespace HUH {

HUH_VULKANLOADER_API std::string ToString(const VkResult in_enum);

}// namespace HUH