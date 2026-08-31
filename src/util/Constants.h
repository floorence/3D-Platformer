#pragma once

#include <glm/ext/vector_float3.hpp>

namespace Constants {
	static constexpr glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
	static constexpr glm::vec3 DOWN = glm::vec3(0.0f, -1.0f, 0.0f);
	static constexpr glm::vec3 FORWARD = glm::vec3(0.0f, 0.0f, -1.0f);
	static constexpr glm::vec3 LEFT = glm::vec3(-1.0f, 0.0f, 0.0f);
	static constexpr glm::vec3 RIGHT = glm::vec3(1.0f, 0.0f, 0.0f);
};
