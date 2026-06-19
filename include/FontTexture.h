#ifndef FONT_TEXTURE_CLASS_H
#define FONT_TEXTURE_CLASS_H

#include"stb/stb_truetype.h"
#include "Texture.h"
#include "VBO.h"

// coordinates of quad within texture atlas as if texture atlas width and height was 1
struct NormalizedCharQuad {
	float x0, x1, y0, y1;
};

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
	std::vector<Vertex> generateVertices(const std::string& text, int x, int y, int w, int lineHeight);
	std::vector<GLuint> generateIndices(std::vector<Vertex>& vertices);
private:
	//constants
	static constexpr int NUM_CHARS = 96;
	static constexpr int ATLAS_WIDTH = 96;
	static constexpr int ATLAS_HEIGHT = 96;

	const float invW = 1.0f / ATLAS_WIDTH;
	const float invH = 1.0f / ATLAS_HEIGHT;

	NormalizedCharQuad charData[NUM_CHARS]; // ascii 32-128
	float normalizedLineHeight = 0.0f;
    const std::string TAG = "FontTexture";

	void processCharData(stbtt_bakedchar* cData);
	void flipBitmap(unsigned char* bytes, int width, int height);
};

#endif