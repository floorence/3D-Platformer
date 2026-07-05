#include"PointLightCamera.h"

PointLightCamera::PointLightCamera(glm::vec3 position, int width, int height)
    : Camera(position, width, height) {}

void PointLightCamera::generateTransforms() {
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

void PointLightCamera::exportCamera(Shader& shader) {
    std::string transformsUniform = "shadowMatrices[0]";

    for (uint i = 0; i < 6; i++) {
        transformsUniform[transformsUniform.size() - 2] = i + '0';
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, transformsUniform.c_str()), 1, GL_FALSE, glm::value_ptr(shadowTransforms[i]));
    }
    glUniform1f(glGetUniformLocation(shader.ID, "far_plane"), farPlane);
    glUniform3f(glGetUniformLocation(shader.ID, "lightPos"), position.x, position.y, position.z);
}