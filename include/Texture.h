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
	void exportTexture(Shader& shader, const char* uniform, GLuint unit); // gives shader this texture, caller has to activate shader beforehand
	void bind(GLuint unit); // bind texture to given unit
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