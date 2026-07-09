#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include"Texture.h"
#include <string>

enum class TextureType {
	Diffuse, Specular
};

class AssetTexture: public Texture {
public:
	TextureType type = TextureType::Diffuse;

	AssetTexture() : Texture() {};
	std::string getUniformName();

	virtual ~AssetTexture() = default;

    AssetTexture(AssetTexture&& other) noexcept;
    AssetTexture& operator=(AssetTexture&& other) noexcept;
protected:
	void setTextureData(unsigned char* bytes, GLenum format, GLenum pixelType, int width, int height);
};

#endif