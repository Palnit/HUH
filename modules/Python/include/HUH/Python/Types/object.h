#pragma once

#include <HUH/Python/definitions.h>

namespace HUH::Py {

class Object {
public:
    template<typename T, typename>
    friend class RefCounter;

    Object() = default;

    // NOLINTNEXTLINE(google-explicit-constructor)
    Object(PyObject* object) : m_object(object) {};

    // NOLINTNEXTLINE(google-explicit-constructor)
    operator PyObject*() const { return m_object; }

    Object& operator++() {
        Py_INCREF(m_object);
        return *this;
    }

    void SafeInc() const { Py_XINCREF(m_object); }

    Object& operator--() {
        Py_DecRef(m_object);
        return *this;
    }

    void SafeDec() const { Py_XDECREF(m_object); }

    explicit operator bool() const { return m_object != nullptr; }
    HUH_NODISCARD bool IsValid() const { return m_object != nullptr; }

protected:
    PyObject* m_object = nullptr;
};

template<typename T, typename = std::enable_if_t<std::is_base_of_v<Object, T>>>
class RefCounter {
public:
    struct Borrow {};
    struct Steal {};

    RefCounter(const T& obj, Borrow) : m_object(std::move(obj)) { m_object.SafeInc(); }
    RefCounter(const T& obj, Steal) : m_object(std::move(obj)) {}

    T& operator*() { return m_object; }
    const T& operator*() const { return m_object; }

    T* operator->() { return &m_object; }
    const T* operator->() const { return &m_object; }

    explicit operator bool() const { return m_object; }

    PyObject* StealRef() {
        PyObject* tmp = m_object;
        m_object.m_object = nullptr;
        return tmp;
    }

    ~RefCounter() { m_object.SafeDec(); }

private:
    T m_object;
};

template<typename T, typename = std::enable_if_t<std::is_base_of_v<Object, T>>>
RefCounter<T> BorrowObj(const T& obj) {
    return RefCounter<T>(obj, typename RefCounter<T>::Borrow());
}

template<typename T, typename = std::enable_if_t<std::is_base_of_v<Object, T>>>
RefCounter<T> StealObj(const T& obj) {
    return RefCounter<T>(obj, typename RefCounter<T>::Steal());
}

}// namespace HUH::Py