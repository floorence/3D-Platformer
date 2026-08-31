#pragma once

#include <glad/glad.h>
#include "mesh/VBO.h"

class VAO {
public:
	VAO();
	~VAO();

	VAO(const VAO&) = delete;
    VAO& operator=(const VAO&) = delete;

    VAO(VAO&& other) noexcept;
    VAO& operator=(VAO&& other) noexcept;

	// Links a VBO Attribute such as a position or color to the VAO
	void linkAttrib(VBO& vbo, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
	void bind();
private:
	GLuint ID;
};
