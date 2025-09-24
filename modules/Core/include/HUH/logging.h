#pragma once
#include <HUH/definitions.h>
#include <filesystem>
#include <format>
#include <iostream>
#include <vector>

namespace HUH {

class LogCategory {
public:
    LogCategory(const std::string& name) : m_name(name) {}

    friend std::ostream& operator<<(std::ostream& os, const LogCategory& category) {
        os << "[" << category.m_name << "]";
        return os;
    }

private:
    std::string m_name;
};

class Logging {
public:
    enum class Level {
        Log,
        Warning,
        Error,
    };

    static std::string ToString(Logging::Level level) {
        switch (level) {
            case Level::Log:
                return "Log";
            case Level::Warning:
                return "Warning";
            case Level::Error:
                return "Error";
            default:
                return "Unknown";
        }
    }

    Logging() = delete;
    template<typename... Args>
    static void Log(const LogCategory& category,
                    Logging::Level level,
                    const std::string& format,
                    const std::string& file_info,
                    Args&&... args) {
        std::cout << category << " " << ToString(level) << ": " << std::vformat(format, std::make_format_args(args...))
                  << file_info << std::endl;
    }

private:
};

}// namespace HUH
#define HUH_LOG(category, level, format, ...)                                                                    \
    HUH::Logging::Log(category, level, format,                                                                   \
                      std::string(" {") + std::filesystem::path(__FILE__).filename().string() + std::string(".") \
                          + std::to_string(__LINE__) + std::string(":") + std::string(__FUNCTION__)              \
                          + std::string("}"),                                                                    \
                      __VA_ARGS__);