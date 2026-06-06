#include"VAO.h"

// Constructor that generates a VAO ID
VAO::VAO() {
	glGenVertexArrays(1, &ID);
}

// Links a VBO Attribute such as a position or color to the VAO
void VAO::linkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset) {
	VBO.bind();
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	VBO.unbind();
}

void VAO::bind() {
	glBindVertexArray(ID);
}

void VAO::unbind() {
	glBindVertexArray(0);
}

VAO::~VAO() {
	glDeleteVertexArrays(1, &ID);
}

VAO::VAO(VAO&& other) noexcept {
	ID = other.ID;
}

VAO& VAO::operator=(VAO&& other) noexcept {
	if (this != &other) {
		ID = other.ID;
	}
	return *this;
}
