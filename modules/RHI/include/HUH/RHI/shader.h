#pragma once
#include <HUH/RHI/fwd.h>
#include <HUH/RHI/rhi_module.h>
#include <HUH/enum_helper.h>
#include <HUH/types.h>

namespace HUH::RHI {
class HUH_RHI_API Shader {
public:
    enum Stage {
        Unknown = 0,
        Fragment = 1 << 0,
        Vertex = 1 << 1,
        Geometry = 1 << 2,
        Compute = 1 << 3,
    };

    friend class Device;

    // TODO refactor to put it into constructor Stage and Entry function name
    virtual bool Init(Stage stage, const std::string& entryFunctionName = "main") = 0;
    virtual void Destroy() = 0;

protected:
    explicit Shader(void* byteCode, Uint64 size) : m_byteCode(byteCode), m_byteCodeSize(size) {};
    virtual ~Shader() = default;
    void* m_byteCode;
    Uint64 m_byteCodeSize;
    Stage m_stage = Unknown;
    std::string m_entryFunctionName;
};

HUH_ENUM_BIT_OPERATORS(Shader::Stage)

}// namespace HUH::RHI