#include"VAO.h"

VAO::VAO() {
	glGenVertexArrays(1, &ID);
}

void VAO::linkAttrib(VBO& vbo, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset) {
	vbo.bind();
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	vbo.unbind();
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
	other.ID = 0;
}

VAO& VAO::operator=(VAO&& other) noexcept {
	if (this != &other) {
		glDeleteVertexArrays(1, &ID);
		ID = other.ID;
		other.ID = 0;
	}
	return *this;
}
