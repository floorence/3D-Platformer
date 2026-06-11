#include"VBO.h"

VBO::VBO() {
	glGenBuffers(1, &ID);
}

// Constructor that generates a Vertex Buffer Object and links it to vertices
VBO::VBO(std::vector<Vertex>& vertices) {
	glGenBuffers(1, &ID);
	bind();
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}

void VBO::linkVertices(std::vector<Vertex>& vertices) {
	bind();
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}

void VBO::bind() {
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

VBO::~VBO() {
    glDeleteBuffers(1, &ID);
}

VBO::VBO(VBO&& other) noexcept {
	ID = other.ID;
	other.ID = 0;
}

VBO& VBO::operator=(VBO&& other) noexcept {
	if (this != &other) {
		glDeleteBuffers(1, &ID);
		ID = other.ID;
		other.ID = 0;
	}
	return *this;
}