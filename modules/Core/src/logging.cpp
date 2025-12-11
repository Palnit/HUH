#include <HUH/types.h>
#include <HUH/logging.h>

namespace HUH {
size_t HUH_CORE_API s_largest_category_size = 0;
LogCategory::LogCategory(std::string&& name) : m_name(name) {
    if (m_name.size() > s_largest_category_size) {
        s_largest_category_size = m_name.size();
    }
}
int LogCategory::GetCategoryWidth() {
    return static_cast<int>((s_largest_category_size + 2) / 4 + 1) * 4;
}
std::ostream& operator<<(std::ostream& os, const LogCategory& category) {
    const int width = LogCategory::GetCategoryWidth();
    os << std::left << std::setw(width) << "[" + category.m_name + "]";
    return os;
}
}// namespace HUH
