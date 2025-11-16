#pragma once
#include <HUH/definitions.h>
#include <filesystem>
#include <format>
#include <iostream>
#include <HUH/console_color.h>

namespace HUH {

class LogCategory {
public:
    explicit LogCategory(const std::string& name) : m_name(name) {
        if (m_name.size() > s_largest_name) {
            s_largest_name = m_name.size();
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const LogCategory& category) {
        os << "[" << category.m_name << "]";
        for (size_t i = 0; i < category.GetNumberOfTabs(); i++) {
            os << "\t";
        }
        return os;
    }

    [[nodiscard]] size_t GetNumberOfTabs() const { return (m_name.size() + 2 - (s_largest_name + 2)) / 8 + 1; }

private:
    std::string m_name;
    inline static size_t s_largest_name = 0;
};

class HUH_API Logging {
public:
    enum Level {
        Log,
        Warning,
        Error,
    };

    static std::string ToStringConsole(Logging::Level level) {
        switch (level) {
            case Level::Log:
                return "[Log]\t\t";
            case Level::Warning:
                return "[Warning]\t";
            case Level::Error:
                return "[Error]\t\t";
            default:
                return "[Unknown]\t";
        }
    }

    static ConsoleColor::Color ToConsoleColor(Logging::Level level) {
        switch (level) {
            case Level::Log:
                return ConsoleColor::Blue;
            case Level::Warning:
                return ConsoleColor::Yellow;
            case Level::Error:
                return ConsoleColor::Red;
            default:
                return ConsoleColor::Reset;
        }
    }

    Logging() = delete;

    template<typename... Args>
    static void AddLog(const LogCategory& category,
                       Logging::Level level,
                       const std::string& format,
                       std::filesystem::path&& file_info,
                       const std::string& file_line,
                       Args&&... args) {
        std::cout << ToConsoleColor(level) << ToStringConsole(level) << ConsoleColor::Cyan << category
                  << ConsoleColor::Reset << std::vformat(format, std::make_format_args(args...)) << "\t["
                  << file_info.lexically_relative(s_huh_path).string() << ":" << file_line << "]" << std::endl;
    }

private:
    inline static std::filesystem::path s_huh_path{HUH_SOURCE_DIR};
};

}// namespace HUH
#define HUH_LOG(category, level, format, ...)                                                                    \
    HUH::Logging::AddLog(category, level, format, __FILE__ ,std::to_string(__LINE__), __VA_ARGS__);

#define HUH_ILOG(category, format, ...) \
    HUH_LOG(category,Logging::Level::Log,format,__VA_ARGS__)

#define HUH_WLOG(category, format, ...) \
    HUH_LOG(category,Logging::Level::Warning,format,__VA_ARGS__)

#define HUH_ELOG(category, format, ...) \
    HUH_LOG(category,Logging::Level::Error,format,__VA_ARGS__)
