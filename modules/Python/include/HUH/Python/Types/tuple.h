#pragma once

#include <HUH/Python/definitions.h>
#include <HUH/Python/Types/object.h>
#include <HUH/Python/Types/accessor.h>

namespace HUH::Py {

class Tuple : public Object {
public:
    class Accessor final : public HUH::Py::Accessor {
    public:
        Accessor(const Tuple* parent, const long index) : HUH::Py::Accessor(parent, index) {
            if (m_parent) {
                m_object = PyTuple_GetItem(*m_parent, m_index);
            }
        }

        Accessor& operator=(const Object& other) {
            PyTuple_SetItem(*m_parent, m_index, other);
            return *this;
        }
    };

    Tuple() = default;

    // NOLINTNEXTLINE(google-explicit-constructor)
    Tuple(PyObject* object) : Object(object) {
        if (!PyTuple_Check(m_object)) {
            HUH_ELOG_THROW(LogPython, "Trying to create a tuple object from non tuple python object")
        }
    }

    Accessor operator[](const long i) { return {this, i}; }

    HUH_NODISCARD long size() const { return PyTuple_Size(m_object); }
};

}// namespace HUH::Py