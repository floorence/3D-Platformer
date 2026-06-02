
#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include<glad/glad.h>
#include<stb/stb_image.h>
#include<string>
#include"Shader.h"

enum class TextureType {
	Diffuse, Specular
};

class Texture {
public:
	TextureType type;

	Texture(const char* image, TextureType texType, GLuint slot, GLenum pixelType, bool specular);

	void initTexture(unsigned char* bytes, GLuint slot, GLenum format, GLenum pixelType, int width, int height);
	void setTexUnit(Shader& shader, const char* uniform, GLuint unit);
	std::string getTypeAsString();
	void bind();
	void unbind();
	void _delete();
private:
	GLuint ID;
	GLuint unit;
};
#endif
