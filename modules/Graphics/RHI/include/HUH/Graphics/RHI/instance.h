#pragma once

#include "HUH/definitions.h"
#include "types.h"
namespace HUH::RHI {

class HUH_API Instance {
public:
    static Instance* Get(RenderApi api);

    Instance(const Instance&) = delete;
    Instance& operator=(const Instance&) = delete;
    Instance(Instance&&) = delete;
    Instance& operator=(Instance&&) = delete;

    virtual ~Instance() = default;
    virtual void Destroy() = 0;

protected:
    Instance() = default;
    virtual bool Init() = 0;
};

}// namespace HUH::RHI