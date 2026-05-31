#include <HUH/Graphics/time.h>

#include "HUH/logging.h"
namespace HUH::Graphics {
std::chrono::time_point<std::chrono::high_resolution_clock> g_start = std::chrono::high_resolution_clock::now();

void Time::Update() {
    auto newTime = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<float> elapsedTime =
        std::chrono::duration_cast<std::chrono::milliseconds>(newTime - m_previousTime);
    DeltaTime = elapsedTime.count();
    m_previousTime = newTime;
    const std::chrono::duration<float> elapsedSec = newTime - m_previousSecTime;
    if (elapsedSec > std::chrono::seconds(1)) {
        Fps = static_cast<float>(m_frames) / elapsedSec.count();
        Ms = elapsedSec.count();
        m_frames = 0;
        m_previousSecTime = newTime;
    }
    m_frames++;
}
}// namespace HUH::Graphics
