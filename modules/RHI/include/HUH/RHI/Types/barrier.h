#pragma once

namespace HUH {
namespace RHI {
class Barrier {
public:
    virtual void Init() = 0;
    virtual void Destroy() = 0;

protected:
    Barrier() = default;
    virtual ~Barrier() = default;
};
}// namespace RHI
}// namespace HUH