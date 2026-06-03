#include <HUH/FileHandling/Image/image_reader.h>

#include "HUH/logging.h"

#include <fstream>
#include <iostream>

#ifdef HUH_IMAGE_READER_AVAILABLE_PNG
#include <HUH/FileHandling/Image/png/png_reader.h>
#endif

namespace HUH::FileHandling {

Image ReadImageFromFile(const std::string& path) {
    std::ifstream ifs;
    ifs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    ifs.open(path, std::ios::binary | std::ios::in | std::ios::ate);
    std::streamsize sizeOfFile = ifs.tellg();
    ifs.seekg(0, std::ios::beg);
    std::vector<Uint8> bytes;
    bytes.resize(sizeOfFile);
    ifs.read(reinterpret_cast<char*>(bytes.data()), sizeOfFile);
    ifs.close();
    return ReadImageFromData(bytes);
}
Image ReadImageFromData(const std::vector<Uint8>& bytes) {
    Image image;
#ifdef HUH_IMAGE_READER_AVAILABLE_PNG
    HUH_TLOG("IS PNG: {}", PngReader::IsPng(bytes))
    if (PngReader::IsPng(bytes)) {
        PngReader reader(bytes);
        reader.ReadPng(image);
    }
#endif

    return image;
}
}// namespace HUH::FileHandling