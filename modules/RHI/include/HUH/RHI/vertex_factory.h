#pragma once
#include "HUH/concepts.h"
#include "HUH/logging.h"
#include "HUH/Math/vector.h"

#include <vector>
#include <HUH/types.h>

namespace HUH::RHI {

class VertexFactory {
public:
    enum class Format { U8, U16, U32, U64, I8, I16, I32, I64, F32, F64 };
    enum class VectorFormat { X, XY, XYZ, XYZW };
    VertexFactory() = default;

    template<typename... T>
    HUH_FORCE_INLINE constexpr void AddVertexStream() {
        m_streams.emplace_back({}, (sizeof(T) + ...));
        (
            [&] {
                if constexpr (HUH::Same<T, Uint8>) {
                    m_streams.back().descriptors.emplace_back(Format::U8, VectorFormat::X);
                } else if constexpr (HUH::Same<T, Uint16>) {
                    m_streams.back().descriptors.emplace_back(Format::U16, VectorFormat::X);
                } else if constexpr (HUH::Same<T, Uint32>) {
                    m_streams.back().descriptors.emplace_back(Format::U32, VectorFormat::X);
                } else if constexpr (HUH::Same<T, Uint64>) {
                    m_streams.back().descriptors.emplace_back(Format::U64, VectorFormat::X);
                } else if constexpr (HUH::Same<T, HUH::Vector2u8>) {
                    m_streams.back().descriptors.emplace_back(Format::U8, VectorFormat::XY);
                } else if constexpr (HUH::Same<T, HUH::Vector2u16>) {
                    m_streams.back().descriptors.emplace_back(Format::U16, VectorFormat::XY);
                } else if constexpr (HUH::Same<T, HUH::Vector2u32>) {
                    m_streams.back().descriptors.emplace_back(Format::U32, VectorFormat::XY);
                } else if constexpr (HUH::Same<T, HUH::Vector2u64>) {
                    m_streams.back().descriptors.emplace_back(Format::U64, VectorFormat::XY);
                } else if constexpr (HUH::Same<T, HUH::Vector3u8>) {
                    m_streams.back().descriptors.emplace_back(Format::U8, VectorFormat::XYZ);
                } else if constexpr (HUH::Same<T, HUH::Vector3u16>) {
                    m_streams.back().descriptors.emplace_back(Format::U16, VectorFormat::XYZ);
                } else if constexpr (HUH::Same<T, HUH::Vector3u32>) {
                    m_streams.back().descriptors.emplace_back(Format::U32, VectorFormat::XYZ);
                } else if constexpr (HUH::Same<T, HUH::Vector3u64>) {
                    m_streams.back().descriptors.emplace_back(Format::U64, VectorFormat::XYZ);
                } else if constexpr (HUH::Same<T, HUH::Vector4u8>) {
                    m_streams.back().descriptors.emplace_back(Format::U8, VectorFormat::XYZW);
                } else if constexpr (HUH::Same<T, HUH::Vector4u16>) {
                    m_streams.back().descriptors.emplace_back(Format::U16, VectorFormat::XYZW);
                } else if constexpr (HUH::Same<T, HUH::Vector4u32>) {
                    m_streams.back().descriptors.emplace_back(Format::U32, VectorFormat::XYZW);
                } else if constexpr (HUH::Same<T, HUH::Vector4u64>) {
                    m_streams.back().descriptors.emplace_back(Format::U64, VectorFormat::XYZW);
                } else if constexpr (HUH::Same<T, Int8>) {
                    m_streams.back().descriptors.emplace_back(Format::I8, VectorFormat::X);
                } else if constexpr (HUH::Same<T, Int16>) {
                    m_streams.back().descriptors.emplace_back(Format::I16, VectorFormat::X);
                } else if constexpr (HUH::Same<T, Int32>) {
                    m_streams.back().descriptors.emplace_back(Format::I32, VectorFormat::X);
                } else if constexpr (HUH::Same<T, Int64>) {
                    m_streams.back().descriptors.emplace_back(Format::I64, VectorFormat::X);
                } else if constexpr (HUH::Same<T, HUH::Vector2i8>) {
                    m_streams.back().descriptors.emplace_back(Format::I8, VectorFormat::XY);
                } else if constexpr (HUH::Same<T, HUH::Vector2i16>) {
                    m_streams.back().descriptors.emplace_back(Format::I16, VectorFormat::XY);
                } else if constexpr (HUH::Same<T, HUH::Vector2i32>) {
                    m_streams.back().descriptors.emplace_back(Format::I32, VectorFormat::XY);
                } else if constexpr (HUH::Same<T, HUH::Vector2i64>) {
                    m_streams.back().descriptors.emplace_back(Format::I64, VectorFormat::XY);
                } else if constexpr (HUH::Same<T, HUH::Vector3i8>) {
                    m_streams.back().descriptors.emplace_back(Format::I8, VectorFormat::XYZ);
                } else if constexpr (HUH::Same<T, HUH::Vector3i16>) {
                    m_streams.back().descriptors.emplace_back(Format::I16, VectorFormat::XYZ);
                } else if constexpr (HUH::Same<T, HUH::Vector3i32>) {
                    m_streams.back().descriptors.emplace_back(Format::I32, VectorFormat::XYZ);
                } else if constexpr (HUH::Same<T, HUH::Vector3i64>) {
                    m_streams.back().descriptors.emplace_back(Format::I64, VectorFormat::XYZ);
                } else if constexpr (HUH::Same<T, HUH::Vector4i8>) {
                    m_streams.back().descriptors.emplace_back(Format::I8, VectorFormat::XYZW);
                } else if constexpr (HUH::Same<T, HUH::Vector4i16>) {
                    m_streams.back().descriptors.emplace_back(Format::I16, VectorFormat::XYZW);
                } else if constexpr (HUH::Same<T, HUH::Vector4i32>) {
                    m_streams.back().descriptors.emplace_back(Format::I32, VectorFormat::XYZW);
                } else if constexpr (HUH::Same<T, HUH::Vector4i64>) {
                    m_streams.back().descriptors.emplace_back(Format::I64, VectorFormat::XYZW);
                } else if constexpr (HUH::Same<T, float>) {
                    m_streams.back().descriptors.emplace_back(Format::F32, VectorFormat::X);
                } else if constexpr (HUH::Same<T, HUH::Vector2f>) {
                    m_streams.back().descriptors.emplace_back(Format::F32, VectorFormat::XY);
                } else if constexpr (HUH::Same<T, HUH::Vector3f>) {
                    m_streams.back().descriptors.emplace_back(Format::F32, VectorFormat::XYZ);
                } else if constexpr (HUH::Same<T, HUH::Vector4f>) {
                    m_streams.back().descriptors.emplace_back(Format::F32, VectorFormat::XYZW);
                } else if constexpr (HUH::Same<T, double>) {
                    m_streams.back().descriptors.emplace_back(Format::F64, VectorFormat::X);
                } else if constexpr (HUH::Same<T, HUH::Vector2d>) {
                    m_streams.back().descriptors.emplace_back(Format::F64, VectorFormat::XY);
                } else if constexpr (HUH::Same<T, HUH::Vector3d>) {
                    m_streams.back().descriptors.emplace_back(Format::F64, VectorFormat::XYZ);
                } else if constexpr (HUH::Same<T, HUH::Vector4d>) {
                    m_streams.back().descriptors.emplace_back(Format::F64, VectorFormat::XYZW);
                }
            }(),
            ...);
    }

private:
    struct Descriptor {
        Format format;
        VectorFormat vectorFormat;
        Uint32 Offset;
    };
    struct Stream {
        std::vector<Descriptor> descriptors;
        Uint32 Stride;
    };
    std::vector<Stream> m_streams;
};

}// namespace HUH::RHI