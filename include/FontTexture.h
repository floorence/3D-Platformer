#ifndef FONT_TEXTURE_CLASS_H
#define FONT_TEXTURE_CLASS_H

#include "Texture.h"

class FontTexture : public Texture {
public:
	FontTexture(const char* ttfFile, GLuint slot, GLenum pixelType = GL_UNSIGNED_BYTE);
private:
    const std::string TAG = "FontTexture";

	void flipBitmap(unsigned char* bytes, int width, int height);
};

#endif