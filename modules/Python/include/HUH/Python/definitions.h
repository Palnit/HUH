#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <HUH/definitions.h>
#include <HUH/logging.h>

inline HUH::LogCategory LogPython("Python");

#define HUH_PYOBJECT_CONSTRUCTORS(Name, CheckFunction) \
    Name() = default; \
    Name(PyObject* object) : Object(object) { \
        if (!Is## Name(m_object)) { \
            m_object = nullptr; \
        } \
    } \
    HUH_FORCE_INLINE static bool Is## Name(PyObject* obj) { return CheckFunction(obj); }
