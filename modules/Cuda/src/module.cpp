#include <HUH/Cuda/module.h>

#include "HUH/Cuda/definitions.h"
#include "HUH/Cuda/device.h"
#include "HUH/Linux/dynamic_library.h"
#include "HUH/string_operations.h"
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

Linker::Linker() {
}

void Linker::AddPtx(const std::string& moduleName) const {
    HUH_JITLIINK_ERR(nvJitLinkAddFile(m_linker, NVJITLINK_INPUT_PTX, moduleName.c_str())) {
        HUH_ELOG(LogCudaLinker, "Error During adding file to linker: {} file: {}", err, moduleName)
    }
}

void Linker::AddLib(const std::string& moduleName) const {
    HUH_JITLIINK_ERR(nvJitLinkAddFile(m_linker, NVJITLINK_INPUT_LIBRARY, moduleName.c_str())) {
        HUH_ELOG(LogCudaLinker, "Error During adding file to linker: {} file: {}", err, moduleName)
    }
}

void Linker::AddObject(const std::string& moduleName) const {
    HUH_JITLIINK_ERR(nvJitLinkAddFile(m_linker, NVJITLINK_INPUT_OBJECT, moduleName.c_str())) {
        HUH_ELOG(LogCudaLinker, "Error During adding file to linker: {} file: {}", err, moduleName)
    }
}

void Linker::AddFatbin(const std::string& moduleName) const {
    HUH_JITLIINK_ERR(nvJitLinkAddFile(m_linker, NVJITLINK_INPUT_FATBIN, moduleName.c_str())) {
        HUH_ELOG(LogCudaLinker, "Error During adding file to linker: {} file: {}", err, moduleName)
    }
}

bool Linker::Init(const Cuda::Device& device) {
    const auto arch = "-arch=sm_" + std::to_string(device.Properties.Major) + std::to_string(device.Properties.Minor);

    const char* lopts[] = {arch.c_str(), "-lto"};

    HUH_JITLIINK_ERR(nvJitLinkCreate(&m_linker, 2, lopts)) {
        HUH_ELOG(LogCudaLinker, "Error during linker initializaton: {}", err)
    }
    return true;
}

void Linker::Complete() const {
    HUH_JITLIINK_ERR(nvJitLinkComplete(m_linker)) {
        HUH_ELOG(LogCudaLinker, "Error during link competition: {}", err);
    }
    size_t logSize = 0;
    HUH_JITLIINK_ERR(nvJitLinkGetErrorLogSize(m_linker, &logSize)) {
        HUH_ELOG(LogCudaLinker, "Error during nvJitLinkGetErrorLogSize {}", err);
    }
    if (logSize != 0) {
        const auto logChar = new char[logSize];
        HUH_JITLIINK_ERR(nvJitLinkGetErrorLog(m_linker, logChar)) {
            HUH_ELOG(LogCudaLinker, "Error during nvJitLinkGetErrorLog {}", err);
        }
        for (auto lines = HUH::Split(logChar, "\n"); auto& line : lines) {
            HUH_ELOG(LogCudaLinker, "Linker Log: {}", line)
        }
        delete[] logChar;
    }

    logSize = 0;
    HUH_JITLIINK_ERR(nvJitLinkGetInfoLogSize(m_linker, &logSize)) {
        HUH_ELOG(LogCudaLinker, "Error during nvJitLinkGetInfoLogSize {}", err);
    }
    if (logSize != 0) {
        char* errorLog = new char[logSize];
        HUH_JITLIINK_ERR(nvJitLinkGetInfoLog(m_linker, errorLog)) {
            HUH_ELOG(LogCudaLinker, "Error during nvJitLinkGetInfoLog {}", err);
        }
        auto lines = HUH::Split(errorLog, "\n");
        for (auto& line : lines) {
            HUH_ILOG(LogCudaLinker, "Linker Log: {}", line)
        }
        delete[] errorLog;
    }
}

void* Linker::GetCubin() {
    if (m_cubin) {
        return m_cubin;
    }
    size_t cubinSize = 0;
    HUH_JITLIINK_ERR(nvJitLinkGetLinkedCubinSize(m_linker, &cubinSize)) {
        HUH_ELOG(LogCudaLinker, "Error during nvJitLinkGetLinkedCubinSize: {}", err);
    }
    m_cubin = malloc(cubinSize);
    if (cubinSize) {
        HUH_JITLIINK_ERR(nvJitLinkGetLinkedCubin(m_linker, m_cubin)) {
            HUH_ELOG(LogCudaLinker, "Error during nvJitLinkGetLinkedCubin: {}", err)
        }
    }
    return m_cubin;
}

Linker::~Linker() {
    if (m_cubin) {
        free(m_cubin);
    }

    if (m_linker) {
        HUH_JITLIINK_ERR(nvJitLinkDestroy(&m_linker)) {
            HUH_ELOG(LogCudaLinker, "Error during nvJitLinkDestroy: {}", err)
        }
    }
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

bool Module::Load(Linker& linker) {
    auto cubinOut = linker.GetCubin();

    HUH_CUDA_ERR(cudaLibraryLoadData(&m_module, cubinOut, nullptr, nullptr, 0, nullptr, nullptr, 0)) {
        HUH_ELOG(LogCuda, "Error Loading Cubin: {}", err)
        return false;
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
        HUH_ELOG(LogCuda, "Error Couldn't load cuda kernel by name: {} err: {}", name, err)
    }
    return Function(cuFunction, this);
}

}// namespace HUH::Cuda
