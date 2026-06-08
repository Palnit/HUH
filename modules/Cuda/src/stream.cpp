#include <HUH/Cuda/stream.h>

#include "HUH/Cuda/definitions.h"

#include <cuda_runtime_api.h>

namespace HUH::Cuda {

Stream::Stream() {
    HUH_CUDA_ERR(cudaStreamCreate(&m_stream)) {
        HUH_ELOG(LogCuda, "Error During Initialization of cuda Stream: {}", err)
    }
}

Stream::~Stream() {
    if (m_stream) {
        HUH_CUDA_ERR(cudaStreamDestroy(m_stream)) {
            HUH_ELOG(LogCuda, "Error During Destruction of cuda Stream: {}", err)
        }
    }
}

}// namespace HUH::Cuda
