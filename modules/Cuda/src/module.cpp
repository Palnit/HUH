#include <HUH/Cuda/module.h>

#include "HUH/Cuda/definitions.h"
#include "HUH/Linux/dynamic_library.h"
#include "HUH/types.h"

#include <cuda_runtime_api.h>

namespace HUH::Cuda {
Function::Function(cudaKernel_t cuFunction, Module* module) : m_func(cuFunction), m_module(module) {
    if (!cuFunction) {
        HUH_WLOG(LogCuda, "Invalid function ptr")
        return;
    }
    const char* name;
    HUH_CUDA_ERR(cudaFuncGetName(&name, m_func)) {
        HUH_ELOG(LogCuda, "Error Getting Function Name: {}", err)
        return;
    }

    size_t paramCount;
    HUH_CUDA_ERR(cudaFuncGetParamCount(m_func, &paramCount)) {
        HUH_ELOG(LogCuda, "Error Getting Function Count: {}", err)
        return;
    }
    for (size_t i = 0; i < paramCount; i++) {
        ParamInfo info{};
        HUH_CUDA_ERR(cudaFuncGetParamInfo(m_func, i, &info.Offset, &info.Size)) {
            HUH_ELOG(LogCuda, "Error Getting Function Count: {}", err)
            return;
        }
        m_params.push_back(info);
    }

    Name = name;
}

Module::Module() {
}

Module::~Module() {
}

bool Module::Load(const std::string& moduleName) {
    HUH_CUDA_ERR(cudaLibraryLoadFromFile(&m_module, moduleName.c_str(), nullptr, nullptr, 0, nullptr, nullptr, 0)) {
        HUH_ELOG(LogCuda, "Error Loading Cuda Module named: {} Err: {}", moduleName, err);
        m_module = nullptr;
    }
    return IsLoaded();
}

std::vector<Function> Module::GetFunctions() {
    HUH::Uint32 functionCount = 0;
    HUH_CUDA_ERR(cudaLibraryGetKernelCount(&functionCount, m_module)) {
        HUH_ELOG(LogCuda, " Error Getting Number of Functions in module: {}", err)
        return {};
    }

    std::vector<cudaKernel_t> tmpFunctions;
    tmpFunctions.resize(functionCount);
    HUH_CUDA_ERR(cudaLibraryEnumerateKernels(tmpFunctions.data(), functionCount, m_module)) {
        HUH_ELOG(LogCuda, "Error Enumerating Functions in module: {}", err)
        return {};
    }

    std::vector<Function> functions;
    for (auto& func : tmpFunctions) {
        functions.push_back(Function(func, this));
    }
    return functions;
}

Function Module::GetFunction(const std::string& name) {
    cudaKernel_t cuFunction = nullptr;
    HUH_CUDA_ERR(cudaLibraryGetKernel(&cuFunction, m_module, name.c_str())) {
        HUH_ELOG(LogCuda, "Error Couldn't load cuda kernel by name: {}", name)
    }
    return Function(cuFunction, this);
}

}// namespace HUH::Cuda
