#pragma once

#include <glad/glad.h>
#include <sys/types.h>

class CubeMapTexture {
public:
    GLuint ID = 0;

    CubeMapTexture(uint width, uint height);
    void bind(GLuint unit);
};
