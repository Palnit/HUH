#include <HUH/Cuda/module.h>

#include "HUH/Cuda/definitions.h"
#include "HUH/Linux/dynamic_library.h"
#include "HUH/types.h"
namespace HUH::Cuda {
Function::Function(CUfunction cuFunction, Module* module) : m_func(cuFunction), m_module(module) {
    const char* name;
    HUH_CU_ERR(cuFuncGetName(&name, m_func)) {
        HUH_ELOG(LogCuda, "Error Getting Function Name: {}", err)
        return;
    }
    Name = name;
}

Module::Module() {
}
Module::~Module() {
}

bool Module::Load(const std::string& moduleName) {
    HUH_CU_ERR(cuModuleLoad(&m_module, moduleName.c_str())) {
        HUH_ELOG(LogCuda, "Error Loading Cuda Module named: {} Err: {}", moduleName, err);
        m_module = nullptr;
    }
    return IsLoaded();
}

std::vector<Function> Module::GetFunctions() {
    HUH::Uint32 functionCount = 0;
    HUH_CU_ERR(cuModuleGetFunctionCount(&functionCount, m_module)) {
        HUH_ELOG(LogCuda, " Error Getting Number of Functions in module: {}", err)
        return {};
    }

    std::vector<CUfunction> tmpFunctions;
    tmpFunctions.resize(functionCount);
    HUH_CU_ERR(cuModuleEnumerateFunctions(tmpFunctions.data(), functionCount, m_module)) {
        HUH_ELOG(LogCuda, "Error Enumerating Functions in module: {}", err)
        return {};
    }

    std::vector<Function> functions;
    for (auto& func : tmpFunctions) {
        functions.push_back(Function(func, this));
    }
    return functions;
}

}// namespace HUH::Cuda
