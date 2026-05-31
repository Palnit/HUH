#include <HUH/FileHandling/Image/png/png_reader.h>
#include <png.h>

namespace HUH::FileHandling {

bool PngReader::IsPng(const std::vector<HUH::Uint8>& bytes) {
    return !png_sig_cmp(bytes.data(), 0, 8);
}

std::vector<HUH::Uint8> PngReader::ReadPng(const std::vector<HUH::Uint8>& bytes) {
    return {};
}
}// namespace HUH::FileHandling