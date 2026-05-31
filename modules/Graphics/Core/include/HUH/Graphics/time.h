#pragma once

// TODO replace chrono
#include <HUH/definitions.h>
#include <HUH/types.h>

#include <chrono>
namespace HUH::Graphics {
extern HUH_GRAPHICS_CORE_API std::chrono::time_point<std::chrono::high_resolution_clock> g_start;

class HUH_GRAPHICS_CORE_API Time {
public:
    Time() = default;
    float DeltaTime = 0;
    float FixedDeltaTime = 0.02;
    float Ms = 0;
    float Fps = 0;

    void Update();

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_previousTime =
        std::chrono::high_resolution_clock::now();
    std::chrono::time_point<std::chrono::high_resolution_clock> m_previousSecTime =
        std::chrono::high_resolution_clock::now();
    Uint64 m_frames = 0;
};
}// namespace HUH::Graphics