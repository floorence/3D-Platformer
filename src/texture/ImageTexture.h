#ifndef IMAGE_TEXTURE_CLASS_H
#define IMAGE_TEXTURE_CLASS_H

#include"AssetTexture.h"

// Texture for raster images
class ImageTexture: public AssetTexture {
public:
	/**
	 * @param image path to image file from project root
	 * @param texType TextureType: diffuse or specular
	 * @param convertToSpecular true if TextureType == specular but the image is actually the diffuse texture and should be
	 * 						    converted to be suitable as a specular texture
	 */
	ImageTexture(const char* image, TextureType texType, GLenum pixelType = GL_UNSIGNED_BYTE, bool convertToSpecular = false);
private:
	// constants
	static constexpr float GAMMA_CORRECT = 1.0 / 2.2f;
	static constexpr float BOOST_COEFFICIENT = 1.5f;

	const std::string TAG = "ImageTexture";

	void flipBitmap(unsigned char* bytes, int width, int height);
};

#endif
