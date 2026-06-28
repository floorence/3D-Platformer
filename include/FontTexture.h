#ifndef FONT_TEXTURE_CLASS_H
#define FONT_TEXTURE_CLASS_H

#include"stb/stb_truetype.h"
#include "Texture.h"
#include "VBO.h"

// coordinates of quad within texture atlas as if texture atlas width and height was 1
struct NormalizedCharQuad {
	float x0, x1, y0, y1;
};

// Texture for ttf font files
class FontTexture : public Texture {
public:
	FontTexture(const char* ttfFile, GLenum pixelType = GL_UNSIGNED_BYTE);

	/**
	 * @brief generate vertices to pass to mesh to draw the text on the screen.
	 * @param text text to show on the screen
	 * @param x x coordinate of top-left corner of where the text should be drawn
	 * @param y y coordinate of top-left corner of where the text should be drawn
	 * @param w maximum width of the text to be drawn, will go on new lines if text is too long
	 * @param lineHeight height to render of tallest character in the font atlas
	 */
	std::vector<Vertex> generateVertices(const std::string& text, int x, int y, int w, int lineHeight);
	std::vector<GLuint> generateIndices(std::vector<Vertex>& vertices);
private:
	//constants
	static constexpr int NUM_CHARS = 96;
	static constexpr int ATLAS_WIDTH = 96;
	static constexpr int ATLAS_HEIGHT = 96;

	/** 
	 * font height given to stbtt_BakeFontBitmap; bakefontbitmap seems to use this value as an upper bound (?) 
	 * for the height of the tallest character in the resulting bitmap.
	 */
	const float fontHeight = 16.0f; 

	NormalizedCharQuad charData[NUM_CHARS]; // ascii 32-127
	float normalizedLineHeight = 0.0f; // height of tallest char as if texture atlas width and height was 1
    const std::string TAG = "FontTexture";

	int getHeightFromBaseline(char c, int charHeight, int lineHeight);
	void processCharData(stbtt_bakedchar* cData);
};

#endif