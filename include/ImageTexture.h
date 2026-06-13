#ifndef IMAGE_TEXTURE_CLASS_H
#define IMAGE_TEXTURE_CLASS_H

#include"Texture.h"

class ImageTexture : public Texture {
public:
	// constructor for rasterized images
	ImageTexture(const char* image, TextureType texType, GLenum pixelType = GL_UNSIGNED_BYTE);
private:
	const std::string TAG = "ImageTexture";

	void flipBitmap(unsigned char* bytes, int width, int height);
};

#endif
