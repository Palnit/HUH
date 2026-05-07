#include <HUH/RHI/pipeline.h>
#include <HUH/enum_helper.h>

namespace HUH {
namespace RHI {}

void RHI::Pipeline::Destroy() {
    for (Buffer* buffer : m_createdBuffers) {
        buffer->Destroy();
        delete buffer;
    }
}

std::string ToString(RHI::Pipeline::Stages type) {
    std::string result = "";
    if (HUH::CheckFlag(type, RHI::Pipeline::Stages::Unknown)) {
        result += "Unknown | ";
    }
    if (HUH::CheckFlag(type, RHI::Pipeline::Stages::TopOfPipe)) {
        result += "TopOfPipe | ";
    }
    if (HUH::CheckFlag(type, RHI::Pipeline::Stages::DrawIndirect)) {
        result += "DrawIndirect | ";
    }
    if (HUH::CheckFlag(type, RHI::Pipeline::Stages::VertexInput)) {
        result += "VertexInput | ";
    }
    if (HUH::CheckFlag(type, RHI::Pipeline::Stages::VertexShader)) {
        result += "VertexShader | ";
    }
    if (HUH::CheckFlag(type, RHI::Pipeline::Stages::TessellationControlShader)) {
        result += "TessellationControlShader | ";
    }
    if (HUH::CheckFlag(type, RHI::Pipeline::Stages::TessellationEvaluationShader)) {
        result += "TessellationEvaluationShader | ";
    }
    if (HUH::CheckFlag(type, RHI::Pipeline::Stages::GeometryShader)) {
        result += "GeometryShader | ";
    }
    if (HUH::CheckFlag(type, RHI::Pipeline::Stages::FragmentShader)) {
        result += "FragmentShader | ";
    }
    if (HUH::CheckFlag(type, RHI::Pipeline::Stages::EarlyFragmentTests)) {
        result += "EarlyFragmentTests | ";
    }
    if (HUH::CheckFlag(type, RHI::Pipeline::Stages::LateFragmentTests)) {
        result += "LateFragmentTests | ";
    }
    if (HUH::CheckFlag(type, RHI::Pipeline::Stages::ColorAttachmentOutput)) {
        result += "ColorAttachmentOutput | ";
    }
    if (HUH::CheckFlag(type, RHI::Pipeline::Stages::ComputeShader)) {
        result += "ComputeShader | ";
    }
    if (HUH::CheckFlag(type, RHI::Pipeline::Stages::Transfer)) {
        result += "Transfer | ";
    }
    if (HUH::CheckFlag(type, RHI::Pipeline::Stages::BottomOfPipe)) {
        result += "BottomOfPipe | ";
    }
    if (HUH::CheckFlag(type, RHI::Pipeline::Stages::Host)) {
        result += "Host | ";
    }
    if (HUH::CheckFlag(type, RHI::Pipeline::Stages::AllGraphics)) {
        result += "AllGraphics | ";
    }
    result = result.substr(0, result.size() - 3);
    return result;
}
}// namespace HUH