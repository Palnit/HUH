#include <HUH/FileHandling/Image/png/png_reader.h>

#include <HUH/FileHandling/Image/definitions.h>

#include <HUH/Math/vector.h>

#include <png.h>

namespace {}

namespace HUH::FileHandling {

PngReader::PngReader(const std::vector<HUH::Uint8>& bytes) : m_bytes(bytes) {
}

bool PngReader::IsPng(const std::vector<HUH::Uint8>& bytes) {
    return !png_sig_cmp(bytes.data(), 0, 8);
}

void PngReader::ReadPng(Image& image) {
    png_structp png_ptr = png_create_read_struct(
        PNG_LIBPNG_VER_STRING, this,
        [](png_structp png_ptr, png_const_charp err) {
            PngReader* reader = static_cast<PngReader*>(png_get_error_ptr(png_ptr));
            if (reader->m_row_pointers) {
                png_free(png_ptr, reader->m_row_pointers);
            }
            HUH_ELOG(LogFileHandlingImage, "PNG Error: {}", err)
        },
        [](auto, png_const_charp err) { HUH_WLOG(LogFileHandlingImage, "PNG Warning: {}", err) });
    if (!png_ptr) {
        return;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return;
    }

    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        return;
    }

    png_set_read_fn(png_ptr, this, PngReader::PngReaderRead);

    png_read_info(png_ptr, info_ptr);

    png_get_IHDR(png_ptr, info_ptr, &image.Size.data[0], &image.Size.data[1], &m_bit_depth, &m_color_type, nullptr,
                 nullptr, nullptr);
    m_channels = png_get_channels(png_ptr, info_ptr);
    png_set_expand(png_ptr);

    if ((m_color_type & PNG_COLOR_MASK_ALPHA) == 0) {
        png_set_add_alpha(png_ptr, 0xff, PNG_FILLER_AFTER);
    }

    m_row_pointers = static_cast<png_bytep*>(png_malloc(png_ptr, image.Size.Height() * sizeof(png_bytep)));

    image.Pixels.resize(image.Size.Height() * image.Size.Width());
    for (Uint32 i = 0; i < image.Size.Height(); ++i) {
        m_row_pointers[i] = reinterpret_cast<Uint8*>(&image.Pixels[i * image.Size.Width()]);
    }
    png_set_rows(png_ptr, info_ptr, m_row_pointers);

#ifdef HUH_LITTLE_ENDIAN
    if (m_bit_depth == 16) {
        png_set_swap(png_ptr);
    }
#endif

    if ((m_color_type & PNG_COLOR_MASK_COLOR) == 0) {
        png_set_gray_to_rgb(png_ptr);
    }

    if (m_bit_depth == 16) {
        png_set_scale_16(png_ptr);
    }

    png_set_interlace_handling(png_ptr);
    png_read_update_info(png_ptr, info_ptr);
    png_read_image(png_ptr, m_row_pointers);
    png_read_end(png_ptr, info_ptr);

    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
}

void PngReader::PngReaderRead(png_structp png_ptr, png_bytep data, png_size_t length) {
    PngReader* reader = static_cast<PngReader*>(png_get_io_ptr(png_ptr));
    // TODO safty cecks for length and data?
    std::memcpy(data, &reader->m_bytes[reader->m_offset], length);
    reader->m_offset += length;
}

}// namespace HUH::FileHandling