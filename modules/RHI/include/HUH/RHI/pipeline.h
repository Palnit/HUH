#pragma once

#include <HUH/RHI/Types/buffer.h>
#include <HUH/RHI/Types/image.h>
#include <HUH/RHI/fwd.h>
#include <HUH/RHI/shader.h>
#include <HUH/enum_define.h>
#include <HUH/types.h>

#include <HUH/FileHandling/Image/image.h>

#include <vector>

namespace HUH {
namespace RHI {
class HUH_RHI_API Pipeline {
public:
    enum class DescriptorTypes {
        Uniform,
        Sampler,
        ImageSampler,
    };

    enum class Stages {
        Unknown = 0,
        TopOfPipe = 1 << 0,
        DrawIndirect = 1 << 1,
        VertexInput = 1 << 2,
        VertexShader = 1 << 3,
        TessellationControlShader = 1 << 4,
        TessellationEvaluationShader = 1 << 5,
        GeometryShader = 1 << 6,
        FragmentShader = 1 << 7,
        EarlyFragmentTests = 1 << 8,
        LateFragmentTests = 1 << 9,
        ColorAttachmentOutput = 1 << 10,
        ComputeShader = 1 << 11,
        Transfer = 1 << 12,
        BottomOfPipe = 1 << 13,
        Host = 1 << 14,
        AllGraphics = 1 << 15
    };

    struct Descriptor {
        DescriptorTypes type;
        Uint32 count;
        Shader::Stage stage;
    };

    struct Initializer {
        RenderPass* renderPass;
        VertexFactory& vertexFactory;
        std::vector<Descriptor> descriptorTypes;
        bool depthTest = false;
    };
    friend class CommandPool;
    friend class Device;
    virtual bool Init(Initializer&& initializer) = 0;
    virtual void Destroy();
    virtual void AddShader(class Shader* shader) = 0;
    virtual Buffer* CreateBuffer(Buffer::Type type, Uint64 Size) = 0;
    virtual Buffer* CreateBuffer(Buffer::Type type, Uint64 Size, Uint64 Binding) = 0;
    virtual HUH::RHI::Image* CreateImage(Image::Type type, const HUH::Vector2u32& size) = 0;
    virtual HUH::RHI::Image* CreateImage(Image::Type type, const HUH::Vector2u32& size, Uint64 Binding) = 0;

protected:
    Pipeline() = default;
    virtual ~Pipeline() = default;
    std::vector<Buffer*> m_createdBuffers;
    std::vector<Image*> m_createdImages;
};
}// namespace RHI
std::string HUH_RHI_API ToString(RHI::Pipeline::Stages stage);
}// namespace HUH

HUH_ENUM_BIT_OPERATORS(HUH::RHI::Pipeline::Stages);
HUH_ENUM_CLASS_EQUAL_OPERATOR(HUH::RHI::Pipeline::Stages);
HUH_ENUM_FORMATER(HUH::RHI::Pipeline::Stages);