#ifndef POINT_LIGHT_CAMERA_H
#define POINT_LIGHT_CAMERA_H

#include"Camera.h"
#include <vector>

class PointLightCamera: public Camera {
public:
	PointLightCamera(glm::vec3 position, int width, int height);

    void generateTransforms();
    void exportCamera(Shader& shader) override;
private:
    std::vector<glm::mat4> shadowTransforms;
};

#endif