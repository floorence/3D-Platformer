#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include"Shader.h"

enum class TextureType {
	Diffuse, Specular
};

class Texture {
public:
	TextureType type = TextureType::Diffuse;

	Texture() = default;
	void initTexture(unsigned char* bytes, GLenum format, GLenum pixelType, int width, int height);
	void setTexUnit(Shader& shader, const char* uniform, GLuint unit);
	void bind(GLuint unit = 0);
	void unbind();
	//virtual std::string getUniformString() const;
	std::string typeToString(TextureType type);

	virtual ~Texture();

	Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;
protected:
	GLuint ID = 0;
};

#endif