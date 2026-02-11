#pragma once

#include <HUH/Python/definitions.h>
#include <HUH/concepts.h>

namespace HUH::Py {

enum BorrowType { BorrowObj };
enum ReferenceType { ReferenceObj };

class Object {
public:
    template<Derived<Object> T>
    friend class RefCounter;

    Object() = default;

    // NOLINTNEXTLINE(google-explicit-constructor)
    Object(PyObject* object) : m_object(object) {};

    // NOLINTNEXTLINE(google-explicit-constructor)
    HUH_FORCE_INLINE operator PyObject*() const { return m_object; }

    HUH_FORCE_INLINE Object& operator++() {
        Py_INCREF(m_object);
        return *this;
    }

    HUH_FORCE_INLINE void SafeInc() const { Py_XINCREF(m_object); }

    HUH_FORCE_INLINE Object& operator--() {
        Py_DecRef(m_object);
        return *this;
    }

    HUH_NODISCARD HUH_FORCE_INLINE static bool IsObject(const PyObject* obj) { return obj != nullptr; }

    HUH_FORCE_INLINE void SafeDec() const { Py_XDECREF(m_object); }

    HUH_FORCE_INLINE explicit operator bool() const { return m_object != nullptr; }
    HUH_NODISCARD HUH_FORCE_INLINE bool IsValid() const { return m_object != nullptr; }

    HUH_NODISCARD HUH_FORCE_INLINE bool HasAttr(const std::string& name) const {
        return PyObject_HasAttrString(m_object, name.c_str());
    }

    HUH_NODISCARD HUH_FORCE_INLINE Object GetAttr(const std::string& name) const {
        return PyObject_GetAttrString(m_object, name.c_str());
    }

    HUH_FORCE_INLINE void SetAttr(const std::string& name, const Object& obj) const {
        PyObject_SetAttrString(m_object, name.c_str(), obj);
    }

    HUH_NODISCARD HUH_FORCE_INLINE bool HasAttr(const Object& name) const { return PyObject_HasAttr(m_object, name); }

    HUH_NODISCARD HUH_FORCE_INLINE Object GetAttr(const Object& name) const { return PyObject_GetAttr(m_object, name); }

    HUH_FORCE_INLINE void SetAttr(const Object& name, const Object& obj) const {
        PyObject_SetAttr(m_object, name, obj);
    }

protected:
    PyObject* m_object = nullptr;
};

template<Derived<Object> T>
class RefCounter {
public:
    RefCounter(const T& obj, BorrowType) noexcept : m_object(std::move(obj)) { m_object.SafeInc(); }
    RefCounter(const T& obj, ReferenceType) noexcept : m_object(std::move(obj)) {}

    RefCounter(PyObject* obj, BorrowType) noexcept : m_object(obj) { m_object.SafeInc(); }
    RefCounter(PyObject* obj, ReferenceType) noexcept : m_object(obj) {}

    RefCounter(RefCounter&& other) noexcept : m_object(std::move(other.m_object)) { other.m_object.m_object = nullptr; }
    RefCounter(RefCounter& other) noexcept : m_object(other.m_object) { m_object.SafeInc(); }

    HUH_FORCE_INLINE T& operator*() { return m_object; }
    HUH_FORCE_INLINE const T& operator*() const { return m_object; }

    HUH_FORCE_INLINE T* operator->() { return &m_object; }
    HUH_FORCE_INLINE const T* operator->() const { return &m_object; }

    HUH_FORCE_INLINE explicit operator bool() const { return m_object; }

    HUH_FORCE_INLINE auto operator[](HUH::Integral auto index)
        requires HUH::HasSubscribe<T, decltype(index)>
    {
        return m_object.operator[](index);
    }

    HUH_FORCE_INLINE PyObject* StealRef() {
        PyObject* tmp = m_object;
        m_object.m_object = nullptr;
        return tmp;
    }

    ~RefCounter() { m_object.SafeDec(); }

private:
    T m_object;
};

template<Derived<Object> T>
RefCounter<T> Borrow(const T& obj) {
    return RefCounter<T>(obj, BorrowObj);
}

template<Derived<Object> T>
RefCounter<T> Steal(const T& obj) {
    return RefCounter<T>(obj, ReferenceObj);
}

template<Derived<Object> T>
RefCounter<T> Borrow(PyObject* obj) {
    return RefCounter<T>(obj, BorrowObj);
}

template<Derived<Object> T>
RefCounter<T> Steal(PyObject* obj) {
    return RefCounter<T>(obj, ReferenceObj);
}

}// namespace HUH::Py