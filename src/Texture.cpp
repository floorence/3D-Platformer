#include"Texture.h"
#include <cmath>

Texture::Texture(const char* image, TextureType texType, GLuint slot, GLenum pixelType, bool specular) {
	// Assigns the type of the texture ot the texture object
	type = texType;

	// Stores the width, height, and the number of color channels of the image
	int widthImg, heightImg, numColCh;
	// Flips the image so it appears right side up
	stbi_set_flip_vertically_on_load(true);
	// Reads the image from a file and stores it in bytes
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);
	Log::log(TAG, "loaded image");
	Log::log(TAG, Log::oss("colour channels: ", numColCh));
	if (specular && numColCh >= 3) {
		// Create 1-channel red-only array
		unsigned char* redChannel = new unsigned char[widthImg * heightImg];
		for (int i = 0; i < widthImg * heightImg; i++) {
			unsigned char r = bytes[i * numColCh + 0];
			unsigned char g = bytes[i * numColCh + 1];
			unsigned char b = bytes[i * numColCh + 2];
			float gray = 0.2126f * r + 0.7152f * g + 0.0722f * b;

			// Apply gamma correction 
			gray = pow(gray / 255.0f, 1.0 / 2.2f) * 255.0f;

			// Apply boost to compensate for dark textures
			gray = std::min(gray * 1.5f, 255.0f);

			// Save as byte
			redChannel[i] = static_cast<unsigned char>(gray);
		}

		initTexture(redChannel, slot, GL_RED, pixelType, widthImg, heightImg);
		delete[] redChannel;
	} else {
		if (type == TextureType::Specular) {
			initTexture(bytes, slot, GL_RED, pixelType, widthImg, heightImg);
		} else {
			initTexture(bytes, slot, GL_RGBA, pixelType, widthImg, heightImg);
		}
	}
	// Deletes the image data as it is already in the OpenGL Texture object
	stbi_image_free(bytes);
}

void Texture::initTexture(unsigned char* bytes, GLuint slot, GLenum format, GLenum pixelType, int width, int height) {
	// Generates an OpenGL texture object
	glGenTextures(1, &ID);
	// Assigns the texture to a Texture Unit
	glActiveTexture(GL_TEXTURE0 + slot);
	unit = slot;
	glBindTexture(GL_TEXTURE_2D, ID);

	// Configures the type of algorithm that is used to make the image smaller or bigger
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Configures the way the texture repeats (if it does at all)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	// Assigns the image to the OpenGL Texture object
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, pixelType, bytes);
	// Generates MipMaps
	glGenerateMipmap(GL_TEXTURE_2D);

	// Unbinds the OpenGL Texture object so that it can't accidentally be modified
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::setTexUnit(Shader& shader, const char* uniform, GLuint unit) {
	// Gets the location of the uniform
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
	// Shader needs to be activated before changing the value of a uniform
	shader.activate();
	// Sets the value of the uniform
	glUniform1i(texUni, unit);
}

std::string Texture::typeToString(TextureType type) {
	switch (type) {
		case TextureType::Diffuse: return "diffuse";
		case TextureType::Specular: return "specular";
	}
	return "";
}

void Texture::bind() {
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() {
	glDeleteTextures(1, &ID);
}

Texture::Texture(Texture&& other) noexcept {
	type = other.type;
	ID = other.ID;
	unit = other.unit;
}

Texture& Texture::operator=(Texture&& other) noexcept {
	if (this != &other) {
		type = other.type;
		ID = other.ID;
		unit = other.unit;
	}
	return *this;
}
