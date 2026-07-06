#include"PointLightCamera.h"

PointLightCamera::PointLightCamera(glm::vec3 position, int width, int height)
    : Camera(position, width, height) {}

void PointLightCamera::generateTransforms() {
    shadowTransforms.clear();
    lookAt(glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f));
    shadowTransforms.push_back(getCameraMatrix());
    lookAt(glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f));
    shadowTransforms.push_back(getCameraMatrix());
    lookAt(glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f));
    shadowTransforms.push_back(getCameraMatrix());
    lookAt(glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f));
    shadowTransforms.push_back(getCameraMatrix());
    lookAt(glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f));
    shadowTransforms.push_back(getCameraMatrix());
    lookAt(glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f));
    shadowTransforms.push_back(getCameraMatrix());
}
