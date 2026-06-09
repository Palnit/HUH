#pragma once
#include "HUH/Math/vector.h"
#include "HUH/definitions.h"
#include "definitions.h"
#include "stream.h"

#include <cuda_runtime.h>
#include <driver_types.h>
#include <string>
#include <vector>

namespace HUH::Cuda {

class Module;
class Stream;

class Function {
public:
    friend class Module;
    std::string Name;

    struct ParamInfo {
        size_t Offset;
        size_t Size;
    } cudaErrorIllegalAddress;

    void SetGrid(const HUH::Vector3ui& gridSize) {
        m_gridSize.x = gridSize.X();
        m_gridSize.y = gridSize.Y();
        m_gridSize.z = gridSize.Z();
    }

    void SetBlock(const HUH::Vector3ui& blockSize) {
        m_blockSize.x = blockSize.X();
        m_blockSize.y = blockSize.Y();
        m_blockSize.z = blockSize.Z();
    }

    void SetStream(Stream* stream) { m_stream = stream; }

    void SetSharedMemory(size_t sharedMemorySize) { m_sharedMemorySize = sharedMemorySize; }

    // Todo Move to stream
    template<typename... Args>
    bool Execute(Args... args) {

        size_t Index = 0;
        size_t Offset = 0;
        bool result = true;
        if (m_params.size() < sizeof...(Args)) {
            return false;
        }
        (
            [&] {
                result = result && m_params[Index].Offset == Offset && m_params[Index].Size == sizeof(args);
                Offset += m_params[Index].Size;
                Index++;
            }(),
            ...);

        if (!result) {
            HUH_ELOG(LogCuda, "Cannot Launch Kernel Incorrect Argument sizes")
            return false;
        }

        void* vargs[] = {static_cast<void*>(&args)...};
        HUH_CUDA_ERR(cudaLaunchKernel(m_func, m_gridSize, m_blockSize, vargs, m_sharedMemorySize,
                                      m_stream ? m_stream->m_stream : nullptr)) {
            HUH_ELOG(LogCuda, "Error Launching Kernel Function  Error: {}", err)
            return false;
        }
        return true;
    }

protected:
    Function(cudaKernel_t cuFunction, Module* module);
    cudaKernel_t m_func;
    Module* m_module = nullptr;
    Stream* m_stream = nullptr;
    dim3 m_gridSize;
    dim3 m_blockSize;
    size_t m_sharedMemorySize = 0;
    std::vector<ParamInfo> m_params;
};

class Module {
public:
    explicit Module();
    ~Module();

    bool Load(const std::string& moduleName);
    HUH_NODISCARD bool IsLoaded() const { return m_module != nullptr; }
    std::vector<Function> GetFunctions();

private:
    cudaLibrary_t m_module = nullptr;
};
}// namespace HUH::Cuda
