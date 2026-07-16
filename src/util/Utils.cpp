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