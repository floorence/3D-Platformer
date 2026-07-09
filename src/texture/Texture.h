#ifndef TEXTURE_H
#define TEXTURE_H

#include"glad/glad.h"

// 2D texture
class Texture {
public:
    Texture();
    void bind();
	void bind(GLuint unit); // bind texture to given unit
	void unbind();

	virtual ~Texture();

	Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;
protected:
    GLuint ID = 0;
};

#endif