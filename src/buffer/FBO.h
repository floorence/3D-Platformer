#ifndef FBO_H
#define FBO_H

#include<glad/glad.h>

class FBO {
public:
	GLuint ID;

	FBO();
	~FBO();

	FBO(const FBO&) = delete;
    FBO& operator=(const FBO&) = delete;

    FBO(FBO&& other) noexcept;
    FBO& operator=(FBO&& other) noexcept;

	void bind();
	void unbind();
    void bindAndClear();
    void unbindAndClear();

    void attachTextureCube(GLuint textureID);
    void attachTexture2D(GLuint textureID);
    void attachRenderBuffer(GLuint rboID);

    void checkStatus();
};

#endif