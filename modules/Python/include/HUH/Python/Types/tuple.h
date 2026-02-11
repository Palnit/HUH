#pragma once

#include <HUH/Python/definitions.h>
#include <HUH/Python/Types/object.h>
#include <HUH/Python/Types/accessor.h>

namespace HUH::Py {

class Tuple : public Object {
public:
    struct TupleAccessor final : public HUH::Py::AccessorType<Object, long> {
        AccessorTypeConstructor(TupleAccessor) {}

        HUH_NODISCARD Object Get() const override { return PyTuple_GetItem(*parent, key); }
        void Set(const Object& type) override { PyTuple_SetItem(*parent, key, type); }
    };

    // NOLINTNEXTLINE(google-explicit-constructor)
    HUH_PYOBJECT_CONSTRUCTORS(Tuple, PyTuple_Check)

    static Tuple Create(const long n) { return PyTuple_New(n); }

    /// Creates a Tuple wrapper
    /// @param obj The Objects to put in the Tuple (Borrows)
    /// @return The created Tuple wrapper
    template<typename... Args>
        requires((HUH::Derived<Args, Object> || HUH::Convertable<Args, PyObject*>) && ...)
    static Tuple Create(const Args&... obj) {
        return PyTuple_Pack(sizeof...(obj), obj...);
    }

    Accessor<TupleAccessor> operator[](HUH::Integral auto i) { return {this, static_cast<long>(i)}; }

    HUH_NODISCARD long size() const { return PyTuple_Size(m_object); }
};

// TODO Finish Named Tuple
class NamedTuple : public Tuple {
public:
    class Descriptor {
    public:
    private:
    };

    NamedTuple() = default;

    // NOLINTNEXTLINE(google-explicit-constructor)
    NamedTuple(PyObject* object) : Tuple(object) {
        if (!IsNamedTuple(m_object)) {
            m_object = nullptr;
        }
    }
    HUH_FORCE_INLINE static bool IsNamedTuple(PyObject* obj) { return PyTuple_Check(obj); }
};

}// namespace HUH::Py