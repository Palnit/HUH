#pragma once
#include "HUH/Math/vector.h"
#include "HUH/definitions.h"

#include <cuda.h>
#include <string>
#include <vector>

namespace HUH::Cuda {

class Module;

class Function {
public:
    friend class Module;
    std::string Name;

protected:
    Function(CUfunction cuFunction, Module* module);
    CUfunction m_func;
    Module* m_module = nullptr;
};

class Module {
public:
    explicit Module();
    ~Module();

    bool Load(const std::string& moduleName);
    HUH_NODISCARD bool IsLoaded() const { return m_module != nullptr; }
    std::vector<Function> GetFunctions();

private:
    CUmodule m_module = nullptr;
};
}// namespace HUH::Cuda
