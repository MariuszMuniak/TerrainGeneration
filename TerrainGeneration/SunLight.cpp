#include "SunLight.h"

glm::vec3 SunLight::GetColor() { return glm::vec3(1.0f, 1.0f, 1.0f); }

glm::vec3 SunLight::GetDirection() { return glm::vec3(0.5f, -0.5f, 0); }
