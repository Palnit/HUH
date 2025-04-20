#ifndef CAMERA_H
#define CAMERA_H
#include <SDL3/SDL_events.h>

#include <glm/glm.hpp>

#include "generic_math.h"
namespace HUH {

class Camera {
public:
    Camera(glm::vec3 eye = Math::ZeroVector,
           glm::vec3 at = Math::ForwardVector,
           glm::vec3 worldUp = Math::UpVector);

    [[nodiscard]] glm::mat4 GetViewMatrix() const;
    [[nodiscard]] glm::mat4 GetProjectionMatrix() const;
    void ProcessKeyboardDown(const SDL_KeyboardEvent& ev);
    void ProcessKeyboardUp(const SDL_KeyboardEvent& ev);
    void ProcessMouseMovement(const SDL_MouseMotionEvent& ev);
    void SetAspect(const float aspect) { m_aspect = aspect; }

    void Update();

private:
    glm::vec3 m_eye;
    glm::vec3 m_worldUp;
    glm::vec3 m_at;
    float m_distance{0.0f};
    float m_v{0.0f};
    float m_u{0.0f};
    float m_speed{16.0f};
    float m_forward{0.0f};
    float m_right{0.0f};
    float m_up{0.0f};
    float m_angle{glm::radians(45.0f)};
    float m_aspect{1.0f};
};
}// namespace HUH
#endif//CAMERA_H
