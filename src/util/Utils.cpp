#include"Utils.h"
#include <fmt/format.h>
#include<glad/glad.h>
#include"Log.h"

float Utils::getBrightness(float r, float g, float b) {
    return R_COEFFICIENT * r + G_COEFFICIENT * g + B_COEFFICIENT * b;
}

void Utils::checkOpenGlErrors() {
    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR) {
        Log::err("checkOpenGlErrors", fmt::format("OpenGL Error Code: {:#x}", err));
    }
}

void Utils::unbindVao() {
	glBindVertexArray(0);
}

void Utils::unbindVbo() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Utils::unbindEbo() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Utils::unbindFbo() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Utils::unbindFboAndClear() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Utils::unbindPbo() {
	glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
}

glm::vec3 Utils::clampVectorLength(glm::vec3 v, float maxLength) {
    float len = glm::length(v);
    return (len > maxLength) ? (v * (maxLength / len)) : v;
}

glm::vec3 Utils::setVectorLength(glm::vec3 v, float desiredLength) {
    float len = glm::length(v);
    return (len != desiredLength) ? (v * (desiredLength / len)) : v;
}
