#pragma once

#include <HUH/types.h>

#include <HUH/Math/vector.h>

#include <png.h>

#include <vector>

namespace HUH::FileHandling {
class PngReader {
public:
    explicit PngReader(const std::vector<HUH::Uint8>& bytes);
    static bool IsPng(const std::vector<HUH::Uint8>& bytes);
    std::vector<HUH::Uint8> ReadPng();

private:
    static void PngReaderRead(png_structp png_ptr, png_bytep data, png_size_t length);
    const std::vector<HUH::Uint8>& m_bytes;
    std::vector<HUH::Vector4u8> m_pixels;
    HUH::Vector2u32 m_size;
    Int32 m_bit_depth;
    Int32 m_color_type;
    Int32 m_channels;
    Int64 m_offset = 0;
    png_bytep* m_row_pointers;
};
}// namespace HUH::FileHandling