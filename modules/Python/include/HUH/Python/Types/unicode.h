#pragma once

#include <HUH/Python/definitions.h>
#include <HUH/Python/Types/object.h>

namespace HUH::Py {
class Unicode : public Object {
public:
    // NOLINTNEXTLINE(google-explicit-constructor)
    HUH_PYOBJECT_CONSTRUCTORS(Unicode, PyUnicode_Check)

    enum class Type { Byte1, Byte2, Byte4 };

    HUH_NODISCARD HUH_FORCE_INLINE HUH::Integral auto GetLength() const { return PyUnicode_GetLength(m_object); }
    HUH_NODISCARD HUH_FORCE_INLINE void* GetData() {
        if (m_data) {
            return m_data;
        }
        m_data = PyUnicode_DATA(m_object);
        return m_data;
    }

private:
    void* m_data = nullptr;
};

}// namespace HUH::Py
