#pragma once

#include <HUH/FileHandling/Image/image.h>
#include <HUH/types.h>
#include <string>
#include <vector>

#include <HUH/lib_defines.h>
namespace HUH::FileHandling {

Image HUH_FILEHANDLING_IMAGE_API ReadImageFromFile(const std::string& path);
Image HUH_FILEHANDLING_IMAGE_API ReadImageFromData(const std::vector<Uint8>& bytes);

}// namespace HUH::FileHandling