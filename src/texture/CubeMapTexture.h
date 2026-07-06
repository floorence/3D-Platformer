#ifndef CUBE_MAP_TEXTURE_H
#define CUBE_MAP_TEXTURE_H

#include<glad/glad.h>
#include<sys/types.h>

class CubeMapTexture {
public:
    GLuint ID = 0;

    CubeMapTexture(uint width, uint height);
    void bind(GLuint unit);
};

#endif