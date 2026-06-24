#include"EBO.h"

EBO::EBO() {
	glGenBuffers(1, &ID);
}

EBO::EBO(std::vector<GLuint>& indices) {
	glGenBuffers(1, &ID);
	bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}

void EBO::linkIndices(const std::vector<GLuint>& indices) {
	bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}

void EBO::bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void EBO::unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

EBO::~EBO() {
	glDeleteBuffers(1, &ID);
}

EBO::EBO(EBO&& other) noexcept {
	ID = other.ID;
	other.ID = 0;
}

EBO& EBO::operator=(EBO&& other) noexcept {
	if (this != &other) {
		glDeleteBuffers(1, &ID);
		ID = other.ID;
		other.ID = 0;
	}
	return *this;
}
