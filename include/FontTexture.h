#ifndef FONT_TEXTURE_CLASS_H
#define FONT_TEXTURE_CLASS_H

#include"stb/stb_truetype.h"
#include "Texture.h"
#include "VBO.h"

class FontTexture : public Texture {
public:
	FontTexture(const char* ttfFile, GLenum pixelType = GL_UNSIGNED_BYTE);

	/**
	 * @brief generate vertices to pass to mesh to draw the text on the screen.
	 * @param text text to show on the screen
	 * @param x x coordinate of top-left corner of where the text should be drawn
	 * @param y y coordinate of top-left corner of where the text should be drawn
	 * @param w maximum width of the text to be drawn, will go on new lines if text is too long
	 */
	std::vector<Vertex> generateVertices(std::string text, int x, int y, int w);
private:
	stbtt_bakedchar charData[96];
    const std::string TAG = "FontTexture";

	void saveCharData(stbtt_bakedchar* charData);
	void flipBitmap(unsigned char* bytes, int width, int height);
};

#endif