#include <HUH/types.h>
#include <HUH/logging.h>

namespace HUH {
size_t HUH_API s_largest_category_size = 0;
LogCategory::LogCategory(std::string&& name) : m_name(name) {
    if (m_name.size() > s_largest_category_size) {
        s_largest_category_size = m_name.size();
    }
}
std::ostream& operator<<(std::ostream& os, const LogCategory& category) {
    const Int64 width = static_cast<Int64>((s_largest_category_size + 2) / 4 + 1) * 4;
    os << std::left << std::setw(width) << "[" + category.m_name + "]";
    return os;
}
}// namespace HUH
