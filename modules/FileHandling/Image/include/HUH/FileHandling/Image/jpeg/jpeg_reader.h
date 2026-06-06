#pragma once
#include <HUH/FileHandling/Image/image.h>

#include <HUH/types.h>

#include <turbojpeg.h>
#include <vector>
namespace HUH::FileHandling {

class JpegReader {
public:
    explicit JpegReader(const std::vector<HUH::Uint8>& bytes);
    ~JpegReader();
    bool IsJpeg();
    void ReadJpeg(Image& im);

private:
    const std::vector<HUH::Uint8>& m_bytes;
    HUH::Vector2i32 m_size;
    tjhandle m_decompressor = nullptr;
};
}// namespace HUH::FileHandling
