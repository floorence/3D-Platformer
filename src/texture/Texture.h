#ifndef TEXTURE_H
#define TEXTURE_H

#include"glad/glad.h"
#include <string>

// 2D texture
class Texture {
public:
    GLuint ID = 0;

    Texture(std::string uniform);
    void bind();
	void bind(GLuint unit); // bind texture to given unit
	void unbind();
    std::string getUniformName();

	virtual ~Texture();

	Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;
protected:
    std::string uniform = "";
};

#endif