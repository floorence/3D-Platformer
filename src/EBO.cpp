#include"EBO.h"

EBO::EBO() {
	glGenBuffers(1, &ID);
}

// Constructor that generates a Elements Buffer Object and links it to indices
EBO::EBO(std::vector<GLuint>& indices) {
	glGenBuffers(1, &ID);
	bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}

void EBO::linkIndices(std::vector<GLuint>& indices) {
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
}

EBO& EBO::operator=(EBO&& other) noexcept {
	if (this != &other) {
		ID = other.ID;
	}
	return *this;
}
