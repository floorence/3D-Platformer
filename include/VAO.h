#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include<glad/glad.h>
#include"VBO.h"

class VAO {
public:
	VAO();
	~VAO();

	VAO(const VAO&) = delete;
    VAO& operator=(const VAO&) = delete;

    VAO(VAO&& other) noexcept;
    VAO& operator=(VAO&& other) noexcept;

	// Links a VBO Attribute such as a position or color to the VAO
	void linkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
	void bind();
	void unbind();
private:
	GLuint ID;
};

#endif
