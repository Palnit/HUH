#include <HUH/string_operations.h>
std::vector<std::string> HUH::Split(std::string s, std::string delimiter) {
    size_t pos_start = 0;
    size_t pos_end;
    size_t delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> tokens;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        tokens.push_back(token);
    }

    tokens.push_back(s.substr(pos_start));
    return tokens;
}
