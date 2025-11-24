#pragma once

#include <vector>
#include <string>
#include <HUH/definitions.h>

namespace HUH {
HUH_CORE_API std::vector<std::string> Split(const std::string& s, const std::string& delimiter);
}// namespace HUH