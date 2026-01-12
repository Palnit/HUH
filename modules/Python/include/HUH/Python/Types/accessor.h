#pragma once

#include <HUH/Python/definitions.h>

namespace HUH::Py {
class Accessor : public Object {
public:
    Accessor(const Object* parent, const long index) : Object(), m_parent(parent), m_index(index) {}

protected:
    const Object* m_parent;
    long m_index;
};
}// namespace HUH::Py