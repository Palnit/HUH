#include <HUH/FileHandling/Image/jpeg/jpeg_reader.h>

#include "HUH/FileHandling/Image/definitions.h"
#include "HUH/logging.h"

namespace HUH::FileHandling {
JpegReader::JpegReader(const std::vector<HUH::Uint8>& bytes) : m_bytes(bytes) {
    m_decompressor = tjInitDecompress();
}

JpegReader::~JpegReader() {
    if (m_decompressor) {
        tjDestroy(m_decompressor);
        m_decompressor = nullptr;
    }
}

bool JpegReader::IsJpeg() {
    if (!m_decompressor) {
        return false;
    }

    int SubSampling = 0;
    int ColorSpace = 0;
    auto res = tjDecompressHeader3(m_decompressor, m_bytes.data(), m_bytes.size(), &m_size.Width(), &m_size.Height(),
                                   &SubSampling, &ColorSpace)
        == 0;
    if (!res) {
        HUH_ELOG(LogFileHandlingImage, "JPEG Header Decompression failed None Jpeg File probably error {}",
                 tj3GetErrorStr(m_decompressor));
    }
    return res;
}

void JpegReader::ReadJpeg(Image& im) {
    im.Size = m_size;
    im.Pixels.resize(m_size.Width() * m_size.Height());
    if (tjDecompress2(m_decompressor, m_bytes.data(), m_bytes.size(), reinterpret_cast<HUH::Uint8*>(im.Pixels.data()),
                      m_size.Width(), 0, m_size.Height(), TJPF_RGBA, TJFLAG_NOREALLOC | TJFLAG_FASTDCT)
        != 0) {
        HUH_ELOG(LogFileHandlingImage, "JPEG Decompression error {}", tj3GetErrorStr(m_decompressor));
    }
}

}// namespace HUH::FileHandling