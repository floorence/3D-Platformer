#include "Texture.h"

void Texture::initTexture(unsigned char* bytes, GLenum format, GLenum pixelType, int width, int height) {
	glGenTextures(1, &ID);
	bind(0);

	// how the image is scaled
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// how the image repeats
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	// assigns the image to the OpenGL Texture object
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, pixelType, bytes);
	glGenerateMipmap(GL_TEXTURE_2D);

	unbind();
}

void Texture::exportTexture(Shader& shader, const char* uniform, GLuint unit) {
	glUniform1i(glGetUniformLocation(shader.ID, uniform), unit);
	glUniform1f(glGetUniformLocation(shader.ID, "material.shininess"), 16);
	bind(unit);
}

void Texture::bind(GLuint unit) {
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

std::string Texture::typeToString(TextureType type) {
	switch (type) {
		case TextureType::Diffuse: return "diffuse";
		case TextureType::Specular: return "specular";
	}
	return "";
}

Texture::~Texture() {
	glDeleteTextures(1, &ID);
}

Texture::Texture(Texture&& other) noexcept
    : ID(other.ID)
{
    other.ID = 0;
}

Texture& Texture::operator=(Texture&& other) noexcept {
	if (this != &other) {
		glDeleteTextures(1, &ID);

		ID = other.ID;
		other.ID = 0;
	}
	return *this;
}

