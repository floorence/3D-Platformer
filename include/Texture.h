#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include<glad/glad.h>
#include<stb/stb_image.h>
#include<string>
#include"stb/stb_truetype.h"
#include <stdlib.h>

#include"Shader.h"

enum class TextureType {
	Diffuse, Specular
};

class Texture {
public:
	TextureType type;

	// constructor for rasterized images
	Texture(const char* image, TextureType texType, GLuint slot, GLenum pixelType = GL_UNSIGNED_BYTE);
	// constructor for ttf font files
	Texture(const char* ttfFile, GLuint slot, GLenum pixelType = GL_UNSIGNED_BYTE);
	~Texture();

	Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

	void initTexture(unsigned char* bytes, GLuint slot, GLenum format, GLenum pixelType, int width, int height);
	void setTexUnit(Shader& shader, const char* uniform, GLuint unit);
	std::string typeToString(TextureType type);
	void bind();
	void unbind();
	void _delete();
private:
	GLuint ID;
	GLuint unit;
	const std::string TAG = "Texture";
	void flipBitmap(unsigned char* bytes, int width, int height);
};

#endif
