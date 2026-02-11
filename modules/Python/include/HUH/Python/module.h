#pragma once

#include <HUH/Python/definitions.h>
#include <HUH/Python/Types/object.h>
#include <string>
#include <utility>
#include <vector>

namespace HUH::Py {

class Module : public Object {
public:
    Module(std::string name, std::string docs) : m_name(std::move(name)), m_docs(std::move(docs)) {
        module.m_name = m_name.c_str();
        module.m_doc = m_docs.c_str();
        m_methods.push_back({nullptr, nullptr, 0, nullptr});
    }

    void AddPyMethodDefManual(const PyMethodDef& method) { m_methods.insert(m_methods.begin(), 1, method); }
    void Finalize() { module.m_methods = m_methods.data(); }

    PyModuleDef module{.m_base = PyModuleDef_HEAD_INIT};

protected:
    std::string m_name;
    std::string m_docs;
    std::vector<PyMethodDef> m_methods;
    std::vector<PyModuleDef_Slot> m_slots;
};

}// namespace HUH::Py

#ifndef HUH_PYTHON_MODULE_IMPORT
#define HUH_PYMODULE_BEGIN(Name, docs) \
    static HUH::Py::Module Name## Module (#Name, docs); \
    PyMODINIT_FUNC PyInit_## Name(void) {

#define HUH_PYMODULE_END(Name) \
    return PyModuleDef_Init(&(Name## Module).module); \
    }
#else
#define HUH_PYMODULE_BEGIN(Name, docs) \
static HUH::Py::Module Name## Module (#Name, docs); \
namespace HUH { \
void Initialize_## Name() {

#define HUH_PYMODULE_END(Name) \
}\
}
#endif
