#pragma once

#include <HUH/Math/vector.h>
#include <HUH/concepts.h>
#include <HUH/types.h>
#include <vector>

namespace HUH::RHI {

class VertexFactory {
public:
    enum class Format { U8, U16, U32, U64, I8, I16, I32, I64, F32, F64 };
    enum class VectorFormat { X, XY, XYZ, XYZW };
    enum class InputRate { Vertex, Instance };
    enum class PolygonMode { Point, Line, Fill };
    VertexFactory() = default;

    friend class Pipeline;
    friend class VulkanPipeline;

    template<auto... T>
        requires(IsMemberPtr<T> && ...)
    HUH_FORCE_INLINE constexpr void AddVertexStream(const InputRate rate = InputRate::Vertex) {
        size_t Stride = 0;
        (
            [&] {
                Stride = sizeof(typename HUH::ClassMemberTypeHelper<T>::StructType);
            }(),
            ...);
        m_streams.emplace_back(static_cast<HUH::Uint32>(Stride), rate);
        (
            [&] {
                AddVertexStreamInternal<typename HUH::ClassMemberTypeHelper<T>::Type>(
                    HUH::ClassMemberTypeHelper<T>::Offset);
            }(),
            ...);
    }

    template<typename... T>
    HUH_FORCE_INLINE constexpr void AddVertexStream(const InputRate rate = InputRate::Vertex) {
        m_streams.emplace_back((sizeof(T) + ...), rate);
        size_t Offset = 0;
        (
            [&] {
                AddVertexStreamInternal<T>(Offset);
                Offset += sizeof(T);
            }(),
            ...);
    }

    void SetTriangulation(PolygonMode tri) { m_polygonMode = tri; }

private:
    template<typename T>
    HUH_FORCE_INLINE constexpr void AddVertexStreamInternal(HUH::Uint32 Offset) {
        if constexpr (HUH::Same<T, Uint8>) {
            m_streams.back().descriptors.emplace_back(Format::U8, VectorFormat::X, Offset);
        } else if constexpr (HUH::Same<T, Uint16>) {
            m_streams.back().descriptors.emplace_back(Format::U16, VectorFormat::X, Offset);
        } else if constexpr (HUH::Same<T, Uint32>) {
            m_streams.back().descriptors.emplace_back(Format::U32, VectorFormat::X, Offset);
        } else if constexpr (HUH::Same<T, Uint64>) {
            m_streams.back().descriptors.emplace_back(Format::U64, VectorFormat::X, Offset);
        } else if constexpr (HUH::Same<T, HUH::Vector2u8>) {
            m_streams.back().descriptors.emplace_back(Format::U8, VectorFormat::XY, Offset);
        } else if constexpr (HUH::Same<T, HUH::Vector2u16>) {
            m_streams.back().descriptors.emplace_back(Format::U16, VectorFormat::XY, Offset);
        } else if constexpr (HUH::Same<T, HUH::Vector2u32>) {
            m_streams.back().descriptors.emplace_back(Format::U32, VectorFormat::XY, Offset);
        } else if constexpr (HUH::Same<T, HUH::Vector2u64>) {
            m_streams.back().descriptors.emplace_back(Format::U64, VectorFormat::XY, Offset);
        } else if constexpr (HUH::Same<T, HUH::Vector3u8>) {
            m_streams.back().descriptors.emplace_back(Format::U8, VectorFormat::XYZ, Offset);
        } else if constexpr (HUH::Same<T, HUH::Vector3u16>) {
            m_streams.back().descriptors.emplace_back(Format::U16, VectorFormat::XYZ, Offset);
        } else if constexpr (HUH::Same<T, HUH::Vector3u32>) {
            m_streams.back().descriptors.emplace_back(Format::U32, VectorFormat::XYZ, Offset);
        } else if constexpr (HUH::Same<T, HUH::Vector3u64>) {
            m_streams.back().descriptors.emplace_back(Format::U64, VectorFormat::XYZ, Offset);
        } else if constexpr (HUH::Same<T, HUH::Vector4u8>) {
            m_streams.back().descriptors.emplace_back(Format::U8, VectorFormat::XYZW, Offset);
        } else if constexpr (HUH::Same<T, HUH::Vector4u16>) {
            m_streams.back().descriptors.emplace_back(Format::U16, VectorFormat::XYZW, Offset);
        } else if constexpr (HUH::Same<T, HUH::Vector4u32>) {
            m_streams.back().descriptors.emplace_back(Format::U32, VectorFormat::XYZW, Offset);
        } else if constexpr (HUH::Same<T, HUH::Vector4u64>) {
            m_streams.back().descriptors.emplace_back(Format::U64, VectorFormat::XYZW, Offset);
        } else if constexpr (HUH::Same<T, Int8>) {
            m_streams.back().descriptors.emplace_back(Format::I8, VectorFormat::X, Offset);
        } else if constexpr (HUH::Same<T, Int16>) {
            m_streams.back().descriptors.emplace_back(Format::I16, VectorFormat::X, Offset);
        } else if constexpr (HUH::Same<T, Int32>) {
            m_streams.back().descriptors.emplace_back(Format::I32, VectorFormat::X, Offset);
        } else if constexpr (HUH::Same<T, Int64>) {
            m_streams.back().descriptors.emplace_back(Format::I64, VectorFormat::X, Offset);
        } else if constexpr (HUH::Same<T, HUH::Vector2i8>) {
            m_streams.back().descriptors.emplace_back(Format::I8, VectorFormat::XY, Offset);
        } else if constexpr (HUH::Same<T, HUH::Vector2i16>) {
            m_streams.back().descriptors.emplace_back(Format::I16, VectorFormat::XY, Offset);
        } else if constexpr (HUH::Same<T, HUH::Vector2i32>) {
            m_streams.back().descriptors.emplace_back(Format::I32, VectorFormat::XY, Offset);
        } else if constexpr (HUH::Same<T, HUH::Vector2i64>) {
            m_streams.back().descriptors.emplace_back(Format::I64, VectorFormat::XY, Offset);
        } else if constexpr (HUH::Same<T, HUH::Vector3i8>) {
            m_streams.back().descriptors.emplace_back(Format::I8, VectorFormat::XYZ, Offset);
        } else if constexpr (HUH::Same<T, HUH::Vector3i16>) {
            m_streams.back().descriptors.emplace_back(Format::I16, VectorFormat::XYZ, Offset);
        } else if constexpr (HUH::Same<T, HUH::Vector3i32>) {
            m_streams.back().descriptors.emplace_back(Format::I32, VectorFormat::XYZ, Offset);
        } else if constexpr (HUH::Same<T, HUH::Vector3i64>) {
            m_streams.back().descriptors.emplace_back(Format::I64, VectorFormat::XYZ, Offset);
        } else if constexpr (HUH::Same<T, HUH::Vector4i8>) {
            m_streams.back().descriptors.emplace_back(Format::I8, VectorFormat::XYZW, Offset);
        } else if constexpr (HUH::Same<T, HUH::Vector4i16>) {
            m_streams.back().descriptors.emplace_back(Format::I16, VectorFormat::XYZW, Offset);
        } else if constexpr (HUH::Same<T, HUH::Vector4i32>) {
            m_streams.back().descriptors.emplace_back(Format::I32, VectorFormat::XYZW, Offset);
        } else if constexpr (HUH::Same<T, HUH::Vector4i64>) {
            m_streams.back().descriptors.emplace_back(Format::I64, VectorFormat::XYZW, Offset);
        } else if constexpr (HUH::Same<T, float>) {
            m_streams.back().descriptors.emplace_back(Format::F32, VectorFormat::X, Offset);
        } else if constexpr (HUH::Same<T, HUH::Vector2f>) {
            m_streams.back().descriptors.emplace_back(Format::F32, VectorFormat::XY, Offset);
        } else if constexpr (HUH::Same<T, HUH::Vector3f>) {
            m_streams.back().descriptors.emplace_back(Format::F32, VectorFormat::XYZ, Offset);
        } else if constexpr (HUH::Same<T, HUH::Vector4f>) {
            m_streams.back().descriptors.emplace_back(Format::F32, VectorFormat::XYZW, Offset);
        } else if constexpr (HUH::Same<T, double>) {
            m_streams.back().descriptors.emplace_back(Format::F64, VectorFormat::X, Offset);
        } else if constexpr (HUH::Same<T, HUH::Vector2d>) {
            m_streams.back().descriptors.emplace_back(Format::F64, VectorFormat::XY, Offset);
        } else if constexpr (HUH::Same<T, HUH::Vector3d>) {
            m_streams.back().descriptors.emplace_back(Format::F64, VectorFormat::XYZ, Offset);
        } else if constexpr (HUH::Same<T, HUH::Vector4d>) {
            m_streams.back().descriptors.emplace_back(Format::F64, VectorFormat::XYZW, Offset);
        }
    }

    struct Descriptor {
        Format format;
        VectorFormat vectorFormat;
        Uint32 Offset;
    };
    struct Stream {
        Uint32 Stride;
        InputRate Rate;
        std::vector<Descriptor> descriptors;
    };
    std::vector<Stream> m_streams;
    PolygonMode m_polygonMode = PolygonMode::Fill;
};

}// namespace HUH::RHI