#include"EBO.h"

// Constructor that generates a Elements Buffer Object and links it to indices
EBO::EBO(std::vector<GLuint>& indices) {
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}

void EBO::bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void EBO::unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

EBO::~EBO() {
	std::cout << "Deleting EBO " << ID << std::endl;
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
