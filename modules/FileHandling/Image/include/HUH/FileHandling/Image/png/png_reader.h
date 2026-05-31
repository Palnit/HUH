#pragma once

#include <HUH/types.h>
#include <vector>

namespace HUH::FileHandling::PngReader {
bool IsPng(const std::vector<HUH::Uint8>& bytes);
std::vector<HUH::Uint8> ReadPng(const std::vector<HUH::Uint8>& bytes);

}// namespace HUH::FileHandling::PngReader