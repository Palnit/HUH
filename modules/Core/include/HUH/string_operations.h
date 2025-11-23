#pragma once

#include <vector>
#include <string>
#include <HUH/definitions.h>

namespace HUH {
HUH_CORE_API std::vector<std::string> Split(std::string s, std::string delimiter);
}// namespace HUH