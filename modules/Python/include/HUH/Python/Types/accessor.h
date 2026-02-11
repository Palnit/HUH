#pragma once

#include <HUH/concepts.h>
#include <HUH/Python/definitions.h>
#include <HUH/Python/Types/fwd.h>
#include <optional>

namespace HUH::Py {
template<typename Stored, typename Key>
struct AccessorType {
    AccessorType(Object* parent, const Key& key) : parent(parent), key(key) {}
    virtual ~AccessorType() = default;
    using StoredType = Stored;
    using KeyType = Key;

    HUH_NODISCARD virtual Stored Get() const = 0;
    virtual void Set(const Stored& value) = 0;

    Object* parent = nullptr;
    Key key;
};

#define AccessorTypeConstructor(Name) \
    Name(Object* parent, const KeyType& key) : HUH::Py::AccessorType<StoredType,KeyType>(parent, key)

template<typename Type>
    requires(HUH::Derived<Type, AccessorType<typename Type::StoredType, typename Type::KeyType>>)
class Accessor {
public:
    using Key = typename Type::KeyType;
    using Stored = typename Type::StoredType;
    Accessor(Object* parent, Key index) : m_accessorType(parent, index) {}

    // NOLINTNEXTLINE(google-explicit-constructor)
    operator Stored() const { return m_accessorType.Get(); }

    // NOLINTNEXTLINE(google-explicit-constructor)
    operator Stored() { return m_accessorType.Get(); }

    HUH_NODISCARD HUH_FORCE_INLINE Stored operator*() { return m_accessorType.Get(); }
    HUH_NODISCARD HUH_FORCE_INLINE Stored operator*() const { return m_accessorType.Get(); }

    // NOLINTNEXTLINE(google-explicit-constructor)
    HUH_FORCE_INLINE operator PyObject*() const
        requires Convertable<Stored, PyObject*>
    {
        return m_accessorType.Get();
    }

    template<HUH::HasSubscribe<Stored>>
    HUH_NODISCARD HUH_FORCE_INLINE decltype(auto) operator[](HUH::Integral auto&& params) const {
        return m_accessorType.Get()[std::forward<decltype(params)>(params)];
    }

    template<HUH::HasSubscribe<Stored>>
    HUH_NODISCARD HUH_FORCE_INLINE decltype(auto) operator[](HUH::Integral auto&& params) {
        return m_accessorType.Get()[std::forward<decltype(params)>(params)];
    }

    template<Convertable<Stored> Object>
    Accessor& operator=(const Object& other) {
        m_accessorType.Set(other);
        return *this;
    }

protected:
    Type m_accessorType;
};
}// namespace HUH::Py