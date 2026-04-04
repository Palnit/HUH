#pragma once

#include <HUH/RHI/fwd.h>
#include <HUH/RHI/dynamic_rhi.h>

namespace HUH::RHI {
class Pipeline {
public:
    struct Initializer {
        std::vector<Format> formats;
        VertexFactory& vertexFactory;
    };

    friend class CommandPool;
    friend class Device;
    virtual bool Init(Initializer&& initializer) = 0;
    virtual void Destroy() = 0;
    virtual void AddShader(class Shader* shader) = 0;

protected:
    Pipeline() = default;
    virtual ~Pipeline() = default;
};
}// namespace HUH::RHI