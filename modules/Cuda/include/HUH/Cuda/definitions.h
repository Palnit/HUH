#pragma once

#include <HUH/logging.h>
#include <cuda.h>
#include <driver_types.h>
#include <nvJitLink.h>

inline HUH::LogCategory LogCuda("Cuda");

namespace HUH {
std::string HUH_CUDA_API ToString(cudaError_t err);
std::string HUH_CUDA_API ToString(CUresult err);
std::string ToString(nvJitLinkResult err);

#define HUH_CUDA_ERR(Func) \
    if (auto err = Func; err != cudaSuccess)

#define HUH_CU_ERR(Func) \
    if (auto err = Func; err != CUDA_SUCCESS)

#define HUH_JITLIINK_ERR(Func) \
    if (auto err = Func; err != NVJITLINK_SUCCESS)

}// namespace HUH

HUH_ENUM_FORMATER(cudaError_t)
HUH_ENUM_FORMATER(CUresult)
HUH_ENUM_FORMATER(nvJitLinkResult)
