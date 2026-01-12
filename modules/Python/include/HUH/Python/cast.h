#pragma once

#include <HUH/Python/definitions.h>
#include <HUH/Python/Types/object.h>
#include <string>

namespace HUH {

// template<typename T>
// T Cast(const HUH::Py::Object& obj) {
//     static_assert(false, "NOT GOOD");
//     return T();
// }

template<typename StringType,
         typename = std::enable_if_t<std::is_same_v<StringType, std::string> || std::is_same_v<StringType, std::wstring>
                                     || std::is_same_v<StringType, std::u16string>
                                     || std::is_same_v<StringType, std::u32string>>>
StringType Cast(const HUH::Py::Object& obj) {
    using CharT = typename StringType::value_type;
    static constexpr size_t utfBytes = 8 * sizeof(CharT);

    if (!PyUnicode_Check(obj)) {
        if (PyBytes_Check(obj)) {
            const char* bytes = PyBytes_AsString(obj);
            if (!bytes) {
                HUH_ELOG_THROW(LogPython, "Unexpected PyBytes_AsString() failure")
            }
            return StringType(bytes, static_cast<size_t>(PyBytes_Size(obj)));
        }

        if (PyByteArray_Check(obj)) {
            const char* bytes = PyByteArray_AsString(obj);
            if (!bytes) {
                HUH_ELOG_THROW(LogPython, "Unexpected PyByteArray_AsString() failure")
            }
            return StringType(bytes, static_cast<size_t>(PyByteArray_Size(obj)));
        }
        HUH_ELOG_THROW(LogPython, "Python object is not string/bytes/byteArray")
    }

    if (utfBytes == 8) {
        Py_ssize_t size = -1;
        const auto* buffer = reinterpret_cast<const CharT*>(PyUnicode_AsUTF8AndSize(obj, &size));
        if (!buffer) {
            HUH_ELOG_THROW(LogPython, "Unexpected PyUnicode_AsUTF8AndSize() failure")
        }
        return StringType(buffer);
    }

    auto bytes_counter =
        Py::StealObj<Py::Object>(PyUnicode_AsEncodedString(obj, utfBytes == 16 ? "utf-16" : "utf-32", nullptr));
    if (!bytes_counter) {
        HUH_ELOG_THROW(LogPython, "Unexpected PyUnicode_AsEncodedString() failure")
    }

    const auto* bytes = reinterpret_cast<const CharT*>(PyBytes_AsString(*bytes_counter));
    if (!bytes) {
        HUH_ELOG_THROW(LogPython, "Unexpected PyBytes_AsString() failure")
    }

    return StringType(++bytes, static_cast<size_t>(PyBytes_Size(*bytes_counter)) - 1);
}
}// namespace HUH