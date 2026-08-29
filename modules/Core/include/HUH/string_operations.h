#pragma once

#include <HUH/Types/array.h>
#include <HUH/definitions.h>
#include <string>
#include <vector>

namespace HUH {
HUH_CORE_API HUH::Array<std::string> Split(const std::string& s, const std::string& delimiter);
}// namespace HUH