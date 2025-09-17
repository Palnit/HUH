#include "HUH/Graphics/OpenGl/camera.h"
#include "SDL3/SDL_events.h"

#include "glm/gtc/matrix_transform.hpp"

#include "HUH/Graphics/generic_structs.h"

namespace HUH::Graphics::OpenGl{

Camera::Camera(glm::vec3 eye, glm::vec3 at, glm::vec3 worldUp)
    : m_eye(eye),
      m_worldUp(worldUp),
      m_at(at),
      m_distance(glm::length(m_at - m_eye)),
      m_v(std::acosf((m_at - m_eye).y / m_distance)),
      m_u(std::atan2f((m_at - m_eye).z, (m_at - m_eye).x)) {}

glm::mat4 Camera::GetViewMatrix() const {
    return glm::lookAt(m_eye, m_at, m_worldUp);
}
glm::mat4 Camera::GetProjectionMatrix() const {
    return glm::perspective(m_angle, m_aspect, 0.01f, 1000.f);
}

void Camera::ProcessKeyboardDown(const SDL_KeyboardEvent& ev) {
    switch (ev.key) {
        case SDLK_LSHIFT:
        case SDLK_RSHIFT:
            if (!ev.repeat) { m_speed /= 4.0f; }
            break;
        case SDLK_W:
            m_forward = 1;
            break;
        case SDLK_S:
            m_forward = -1;
            break;
        case SDLK_A:
            m_right = -1;
            break;
        case SDLK_D:
            m_right = 1;
            break;
        case SDLK_E:
            m_up = 1;
            break;
        case SDLK_Q:
            m_up = -1;
            break;
    }
}
void Camera::ProcessKeyboardUp(const SDL_KeyboardEvent& ev) {
    switch (ev.key) {
        case SDLK_LSHIFT:
        case SDLK_RSHIFT:
            m_speed *= 4.0f;
            break;
        case SDLK_W:
        case SDLK_S:
            m_forward = 0;
            break;
        case SDLK_A:
        case SDLK_D:
            m_right = 0;
            break;
        case SDLK_E:
        case SDLK_Q:
            m_up = 0;
            break;
    }
}

void Camera::ProcessMouseMovement(const SDL_MouseMotionEvent& ev) {
    if (ev.state & SDL_BUTTON_LMASK) {
        float du = ev.xrel * 0.01f;
        float dv = ev.yrel * 0.01f;

        m_u += du;
        m_v = glm::clamp<float>(m_v + dv, 0.1f, 3.1f);
    }
    if (ev.state & SDL_BUTTON_RMASK) {
        m_distance *= std::powf(0.9f, ev.yrel / 50.0f);
    }
}

void Camera::Update() {
    glm::vec3 lookDirection(std::cosf(m_u) * std::sinf(m_v), std::cosf(m_v),
                            std::sinf(m_u) * std::sinf(m_v));
    m_eye = m_at - m_distance * lookDirection;
    glm::vec3 right = glm::normalize(glm::cross(lookDirection, m_worldUp));
    glm::vec3 forward = glm::normalize(glm::cross(m_worldUp, right));
    glm::vec3 deltaPos =
        (m_forward * forward + m_right * right + m_up * m_worldUp) * m_speed
        * Time::DeltaTimeF();
    m_eye += deltaPos;
    m_at += deltaPos;
}
}// namespace HUH