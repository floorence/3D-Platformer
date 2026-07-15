#include"PBO.h"

PBO::PBO() {
	glGenBuffers(1, &ID);
    bind();
    glBufferData(GL_PIXEL_PACK_BUFFER, 4, nullptr, GL_STREAM_READ);
}

void PBO::bind() {
	glBindBuffer(GL_PIXEL_PACK_BUFFER, ID);
}

void PBO::unbind() {
	glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
}

PBO::~PBO() {
	glDeleteBuffers(1, &ID);
}

PBO::PBO(PBO&& other) noexcept {
	ID = other.ID;
	other.ID = 0;
}

PBO& PBO::operator=(PBO&& other) noexcept {
	if (this != &other) {
		glDeleteBuffers(1, &ID);
		ID = other.ID;
		other.ID = 0;
	}
	return *this;
}
