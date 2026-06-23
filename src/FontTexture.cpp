#include <algorithm>
#define STB_TRUETYPE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include<fstream>
#include "FontTexture.h"
#include<string.h>
#include "Log.h"
#include<fmt/format.h>
#include"stb/stb_image_write.h"

FontTexture::FontTexture(const char* ttfFile, GLenum pixelType) {
    std::ifstream file(ttfFile, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        Log::err(TAG, "Failed to open font file.");
        return;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
	unsigned char* ttf_buffer = new unsigned char[size];
    
    if (!file.read(reinterpret_cast<char*>(ttf_buffer), size)) {
		Log::err(TAG, "Failed to read font file.");
		delete[] ttf_buffer;
		return;
	}

	int atlas_w = ATLAS_WIDTH;
    int atlas_h = ATLAS_HEIGHT;
	
    unsigned char* bitmap_pixels = new unsigned char[atlas_w * atlas_h];
	stbtt_bakedchar cData[NUM_CHARS]; // ascii 32-128

    float font_height = fontHeight;
    int result = stbtt_BakeFontBitmap(ttf_buffer, 0, font_height, 
                                      bitmap_pixels, atlas_w, atlas_h, 
                                      32, NUM_CHARS, cData);
    
    if (result <= 0) {
        Log::err(TAG, "stbtt_BakeFontBitmap failed. does the font fit into the atlas matrix?");
    } else {
		//stbi_write_png("assets/font_dev.png", atlas_w, atlas_h, 1, bitmap_pixels, atlas_w);
		processCharData(cData);
		//flipBitmap(bitmap_pixels, atlas_w, atlas_h);
		initTexture(bitmap_pixels, GL_RED, pixelType, atlas_w, atlas_h);
	}
	delete[] ttf_buffer;
	delete[] bitmap_pixels;
}

std::vector<Vertex> FontTexture::generateVertices(const std::string& text, int x, int y, int w, int lineHeight) {
	std::vector<Vertex> vertices;
	float scale = lineHeight / normalizedLineHeight;
	float spacing = lineHeight / 10.0f;
	float spaceWidth = lineHeight / 6.0f;
	float tabWidth = lineHeight * 2;
	// current x and y of the BOTTOM LEFT of the character
	int currX = x;
	int currY = y - lineHeight;
	for (unsigned long i = 0; i < text.size(); i++) {

		// handle "special" characters (not ascii 32 - 127)
		if (text[i] == '\n') {
			currX = x;
			currY += lineHeight;
			continue;
		} else if (text[i] == '\t') {
			currX += tabWidth;
			continue;
		} else if (text[i] < ' ' /* char type can't be > 127 so no need to check here */) {
			Log::warn(TAG, fmt::format("unrecognized character: {}", text[i]));
			continue;
		}

		NormalizedCharQuad c = charData[text[i] - ' '];
		int width = (c.x1 - c.x0) * scale;
		int height = (c.y1 - c.y0) * scale;

		//Log::log(TAG, Log::oss(text[i], " c: ", text[i] - ' ', " width: ", width, " height: ", height));

		// constrain width
		if (currX + width > x + w) {
			currX = x;
			currY += lineHeight;
		}

		if (text[i] == ' ') {
			currX += spaceWidth;
			continue;
		}

		int baselineHeight = getHeightFromBaseline(text[i], height, lineHeight);

		Vertex topLeft = {
			glm::vec3(currX, currY - baselineHeight, 0.0f),
			glm::vec3(0.0f),
			glm::vec3(0.0f),
			glm::vec2(c.x0, c.y0)
		};
		Vertex topRight = {
			glm::vec3(currX + width, currY - baselineHeight, 0.0f),
			glm::vec3(0.0f),
			glm::vec3(0.0f),
			glm::vec2(c.x1, c.y0)
		};
		Vertex botLeft = {
			glm::vec3(currX, currY - baselineHeight + height, 0.0f),
			glm::vec3(0.0f),
			glm::vec3(0.0f),
			glm::vec2(c.x0, c.y1)
		};
		Vertex botRight = {
			glm::vec3(currX + width, currY - baselineHeight + height, 0.0f),
			glm::vec3(0.0f),
			glm::vec3(0.0f),
			glm::vec2(c.x1, c.y1)
		};

		vertices.insert(vertices.end(), {topLeft, topRight, botLeft, botRight});

		currX += width + spacing;
		// Log::log(TAG, Log::oss("\t topLeft: ", topLeft.to_string(), " topRight: ", topRight.to_string(), " botLeft: ", botLeft.to_string(), " botRight: ", botRight.to_string()));
		// Log::log(TAG, Log::oss("\tcurrX: ", currX, " currY: ", currY));
	}
	
	return vertices;
}

std::vector<GLuint> FontTexture::generateIndices(std::vector<Vertex>& vertices) {
	std::vector<GLuint> indices;
	for (uint i = 0; i < vertices.size(); i += 4 /* 4 vertices per quad */ ) {
		indices.insert(indices.end(), {i, i + 3, i + 2, i, i + 3, i + 1});
	}
	return indices;
}

int FontTexture::getHeightFromBaseline(char c, int charHeight, int lineHeight) {
	if (c == 'g' || c == 'j' || c == 'p' || c == 'q' || c == 'y') { // delimiters
		return charHeight - (charHeight / 4.0f);
	} else if (c == '+' || c == '-' || c == '<' || c == '=' || c == '>' || c == '~') { // center vertically
		return lineHeight / 2.0f - charHeight / 2.0f;
	} else if (c == '\"' || c == '\'' || c == '*' || c == '^') { // top aligned
		return lineHeight;
	}
	return charHeight; // bottom aligned
}

void FontTexture::processCharData(stbtt_bakedchar* cData) {
	for (int i = 0; i < NUM_CHARS; i++) {
		stbtt_bakedchar c = cData[i];
		charData[i].x0 = (float)c.x0 / ATLAS_WIDTH;
		charData[i].x1 = (float)c.x1 / ATLAS_WIDTH;
		charData[i].y0 = (float)c.y0 / ATLAS_HEIGHT;
		charData[i].y1 = (float)c.y1 / ATLAS_HEIGHT;
		normalizedLineHeight = std::max(normalizedLineHeight, charData[i].y1 - charData[i].y0);
	}
}

void FontTexture::flipBitmap(unsigned char* bytes, int width, int height) {
	unsigned char* temp_row = new unsigned char[width];
	for (int i = 0; i < height / 2; ++i) {
		unsigned char* row1 = bytes + i * width;
		unsigned char* row2 = bytes + (height - 1 - i) * width;
		
		// Swap rows
		memcpy(temp_row, row1, width);
		memcpy(row1, row2, width);
		memcpy(row2, temp_row, width);
	}
	delete[] temp_row;
}
