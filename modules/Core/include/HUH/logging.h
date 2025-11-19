#pragma once
#include <HUH/definitions.h>
#include <filesystem>
#include <format>
#include <iostream>
#include <HUH/console_color.h>

namespace HUH {

class HUH_API LogCategory {
public:
    explicit LogCategory(std::string&& name);
    friend HUH_API std::ostream& operator<<(std::ostream& os, const LogCategory& category);

    HUH_NODISCARD size_t GetNumberOfTabs() const;

private:
    std::string m_name;
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
                return "[Log]";
            case Level::Warning:
                return "[Warning]";
            case Level::Error:
                return "[Error]";
            default:
                return "[Unknown]";
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
        std::cout << ToConsoleColor(level) << std::left << std::setw(12) << ToStringConsole(level) << ConsoleColor::Cyan
                  << category << ConsoleColor::Reset << std::vformat(format, std::make_format_args(args...)) << "\t["
                  << file_info.lexically_relative(s_huh_path).string() << ":" << file_line << "]" << std::endl;
    }

private:
    inline static std::filesystem::path s_huh_path{HUH_SOURCE_DIR};
};

}// namespace HUH
#define HUH_LOG(category, level, format, ...)                                                                    \
    HUH::Logging::AddLog(category, level, format, __FILE__ ,std::to_string(__LINE__) __VA_OPT__(,) __VA_ARGS__);

#define HUH_ILOG(category, format, ...) \
    HUH_LOG(category,Logging::Level::Log,format __VA_OPT__(,) __VA_ARGS__)

#define HUH_WLOG(category, format, ...) \
    HUH_LOG(category,Logging::Level::Warning,format __VA_OPT__(,) __VA_ARGS__)

#define HUH_ELOG(category, format, ...) \
    HUH_LOG(category,Logging::Level::Error,format __VA_OPT__(,) __VA_ARGS__)
