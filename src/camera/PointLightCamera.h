#pragma once

#include "camera/Camera.h"
#include <vector>

class PointLightCamera: public Camera {
public:
    std::vector<glm::mat4> shadowTransforms;

	PointLightCamera(glm::vec3 position, int width, int height);

    void generateTransforms();
};
