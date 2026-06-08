#pragma once
#include <driver_types.h>

namespace HUH::Cuda {
class Stream {
public:
    friend class Function;
    Stream();
    ~Stream();

protected:
    cudaStream_t m_stream = nullptr;
};
}// namespace HUH::Cuda
