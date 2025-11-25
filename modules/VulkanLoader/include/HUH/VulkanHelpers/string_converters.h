#pragma once
#include <HUH/definitions.h>
#include <string>
enum VkResult;

namespace HUH {

HUH_VULKANLOADER_API std::string ToString(const VkResult in_enum);

}// namespace HUH