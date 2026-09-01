#pragma once

#include <glad/glad.h>

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
    void bindAndClear();

    void attachTextureCube(GLuint textureID);
    void attachTexture2D(GLuint textureID, int attachment = 0);
    void attachRenderBuffer(GLuint rboID);

    void checkStatus();
};
