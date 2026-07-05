#ifndef CUBE_MAP_TEXTURE_H
#define CUBE_MAP_TEXTURE_H

#include "shader/Shader.h"

class CubeMapTexture {
public:
    GLuint ID = 0;

    CubeMapTexture(uint width, uint height);
    void exportTexture(Shader& shader, const char* uniform, GLuint unit);
};

#endif