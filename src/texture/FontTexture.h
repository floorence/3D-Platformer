#pragma once

#include "stb/stb_truetype.h"
#include "texture/AssetTexture.h"
#include "mesh/VBO.h"

// coordinates of quad within texture atlas as if texture atlas width and height was 1
struct NormalizedCharQuad {
	float x0, x1, y0, y1;
};

// Texture for ttf font files
class FontTexture: public AssetTexture {
public:
	FontTexture(const char* ttfFile, GLenum pixelType = GL_UNSIGNED_BYTE);

	/**
	 * @brief generate vertices to pass to mesh to draw the text on the screen.
	 * @param text text to show on the screen
	 * @param x x coordinate of top-left corner of where the text should be drawn
	 * @param y y coordinate of top-left corner of where the text should be drawn
	 * @param lineHeight height to render of tallest character in the font atlas
	 * @param maxWidth optional maximum width of the text to be drawn, will go on new lines if text is too long
	 * @param center whether or not each character should be centered vertically within its quad, no matter the character
	 */
	std::vector<Vertex> generateVertices(const std::string& text, float x, float y, float lineHeight, float maxWidth = -1, bool center = false);
	std::vector<GLuint> generateIndices(std::vector<Vertex>& vertices);

	/**
	 * @brief gets the size in pixels of the given text of lineHeight, optionally constrained to maxWidth
	 *        (in which case the x component will always be less than or equal to maxWidth)
	 * @return size of the text; first = width, second = height
	 */
	std::pair<float, float> getSize(const std::string& text, float lineHeight, float maxWidth = -1);
	/** @return specific width and height of char */
	std::pair<float, float> getSizeOfChar(char c, float lineHeight);
private:
	//constants
	static constexpr int NUM_CHARS = 96;
	static constexpr int ATLAS_WIDTH = 96;
	static constexpr int ATLAS_HEIGHT = 96;
	const float OVERFLOW_ALLOWANCE = 0.001; // account for floating point precision error

	/** 
	 * font height given to stbtt_BakeFontBitmap; bakefontbitmap seems to use this value as an upper bound (?) 
	 * for the height of the tallest character in the resulting bitmap.
	 */
	const float fontHeight = 16.0f; 

	NormalizedCharQuad charData[NUM_CHARS]; // ascii 32-127
	float normalizedLineHeight = 0.0f; // height of tallest char as if texture atlas width and height was 1
    const std::string TAG = "FontTexture";

	float getHeightFromBaseline(char c, float charHeight, float lineHeight);
	void scaleCharQuad(NormalizedCharQuad c, float scale, float* width, float* height);
	void processCharData(stbtt_bakedchar* cData);
	/** slightly scuffed workaround to have generateVertices and getSize be able to use the same function */
	void processTextRequest(const std::string& text, float x, float y, float lineHeight, float maxWidth, bool center, std::vector<Vertex>* vertexData, std::pair<float, float>* sizeData);
};
