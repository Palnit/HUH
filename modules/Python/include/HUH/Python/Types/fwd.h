#pragma once

namespace HUH::Py {
class Object;
class Tuple;
class Unicode;

template<typename Stored, typename Key>
struct AccessorType;

template<typename Type>
    requires(HUH::Derived<Type, AccessorType<typename Type::StoredType, typename Type::KeyType>>)
class Accessor;

}// namespace HUH::Py
