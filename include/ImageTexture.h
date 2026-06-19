#ifndef IMAGE_TEXTURE_CLASS_H
#define IMAGE_TEXTURE_CLASS_H

#include"Texture.h"

// Texture for rasterized images
class ImageTexture : public Texture {
public:
	ImageTexture(const char* image, TextureType texType, GLenum pixelType = GL_UNSIGNED_BYTE, bool convertToSpecular = false);
private:
	// constants
	static constexpr float R_COEFFICIENT = 0.2126f;
	static constexpr float G_COEFFICIENT = 0.7152f;
	static constexpr float B_COEFFICIENT = 0.0722f;
	static constexpr float GAMMA_CORRECT = 1.0 / 2.2f;
	static constexpr float BOOST_COEFFICIENT = 1.5f;

	const std::string TAG = "ImageTexture";

	void flipBitmap(unsigned char* bytes, int width, int height);
};

#endif
