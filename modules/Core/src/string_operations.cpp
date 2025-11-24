#include <HUH/string_operations.h>
std::vector<std::string> HUH::Split(const std::string& s, const std::string& delimiter) {
    if (s.empty()) {
        return {};
    }
    size_t pos_start = 0;
    size_t pos_end;
    const size_t delim_len = delimiter.length();
    std::vector<std::string> tokens;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        std::string token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        tokens.push_back(token);
    }

    tokens.push_back(s.substr(pos_start));
    return tokens;
}
