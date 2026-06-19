#pragma once
#include "HUH/Math/vector.h"
#include "HUH/definitions.h"
#include "definitions.h"
#include "stream.h"

#include <cuda_runtime.h>
#include <nvJitLink.h>
#include <string>
#include <vector>

namespace HUH::Cuda {
class Device;

class Module;
class Stream;

class Function {
public:
    Function() = default;
    friend class Module;
    std::string Name;

    struct ParamInfo {
        size_t Offset;
        size_t Size;
    } cudaErrorIllegalAddress;

    explicit operator bool() const { return m_func; }

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

#if defined(HUH_DEBUG) and false
        size_t Index = 0;
        size_t Offset = 0;
        bool result = true;
        if (m_params.size() < sizeof...(Args))
            args {
                HUH_ELOG(LogCuda, "Incorrect Number of argument launches")
                return false;
            }
        (
            [&] {
                auto AlignReq = alignof(decltype(args));
                Offset += (AlignReq - (Offset % AlignReq)) % AlignReq;
                result = result && m_params[Index].Offset == Offset && m_params[Index].Size == sizeof(args);
                Offset += sizeof(args);
                Index++;
            }(),
            ...);

        if (!result) {
            HUH_WLOG(LogCuda, "Cannot Launch Kernel Named: {} Incorrect Argument sizes", Name)
            Index = 0;
            Offset = 0;
            (
                [&] {
                    const auto AlignReq = alignof(decltype(args));
                    Offset += (AlignReq - (Offset % AlignReq)) % AlignReq;
                    HUH_WLOG(LogCuda, "\tFunction Arg {} Supplied Offset {} Size {} | Expected Offset {} Size {}",
                             Index, Offset, sizeof(args), m_params[Index].Offset, m_params[Index].Size,
                             alignof(decltype(args)))
                    Offset += sizeof(args);
                    Index++;
                }(),
                ...);
        }
#endif

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
    cudaKernel_t m_func = nullptr;
    Module* m_module = nullptr;
    Stream* m_stream = nullptr;
    dim3 m_gridSize;
    dim3 m_blockSize;
    size_t m_sharedMemorySize = 0;
    std::vector<ParamInfo> m_params;
};

inline LogCategory LogCudaLinker("CudaLinker");

class Linker {
public:
    friend class Module;
    Linker();
    void AddPtx(const std::string& moduleName) const;
    void AddLib(const std::string& moduleName) const;
    void AddObject(const std::string& moduleName) const;
    void AddFatbin(const std::string& moduleName) const;
    bool Init(const Cuda::Device& device);
    void Complete() const;
    void* GetCubin();
    ~Linker();

private:
    nvJitLinkHandle m_linker = nullptr;
    void* m_cubin = nullptr;
};

class Module {
public:
    explicit Module();
    ~Module();

    bool Load(const std::string& moduleName);
    bool Load(Linker& linker);
    HUH_NODISCARD bool IsLoaded() const { return m_module != nullptr; }
    std::vector<Function> GetFunctions();
    Function GetFunction(const std::string& name);

private:
    cudaLibrary_t m_module = nullptr;
};
}// namespace HUH::Cuda
