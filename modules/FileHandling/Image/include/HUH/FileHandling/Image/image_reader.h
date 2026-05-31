#pragma once

#include <HUH/FileHandling/Image/image.h>
#include <HUH/types.h>
#include <string>
#include <vector>

namespace HUH::FileHandling {

Image ReadImageFromFile(const std::string& path);
Image ReadImageFromData(const std::vector<Uint8>& bytes);

}// namespace HUH::FileHandling