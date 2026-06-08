#pragma once

#include <HUH/logging.h>
#include <cuda.h>
#include <driver_types.h>

inline HUH::LogCategory LogCuda("Cuda");

namespace HUH {
std::string ToString(cudaError_t err);
std::string ToString(CUresult err);

#define HUH_CUDA_ERR(Func) \
    if (auto err = Func; err != cudaSuccess)

#define HUH_CU_ERR(Func) \
    if (auto err = Func; err != CUDA_SUCCESS)

}// namespace HUH

HUH_ENUM_FORMATER(cudaError_t)
HUH_ENUM_FORMATER(CUresult)
