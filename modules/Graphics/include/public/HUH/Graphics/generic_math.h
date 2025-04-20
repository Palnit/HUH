#ifndef GENERIC_MATH_H
#define GENERIC_MATH_H

#include <glm/vec3.hpp>

namespace HUH::Math {

static constexpr auto ZeroVector = glm::vec3(0.0f, 0.0f, 0.f);
static constexpr auto OneVector = glm::vec3(1.0f, 1.0f, 1.f);
static constexpr auto UpVector = glm::vec3(0.0f, 1.0f, 0.f);
static constexpr auto DownVector = glm::vec3(0.0f, -1.0f, 0.f);
static constexpr auto ForwardVector = glm::vec3(0.0f, 0.0f, -1.f);
static constexpr auto BackwardVector = glm::vec3(0.0f, 0.0f, 1.f);
// static constexpr auto RightVector = glm::vec3(0.0f, 1.0f, 0.f);
// static constexpr auto LeftVector = glm::vec3(0.0f, -1.0f, 0.f);

}// namespace HUH::Math

#endif//GENERIC_MATH_H
