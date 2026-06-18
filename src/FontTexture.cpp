#include <algorithm>
#define STB_TRUETYPE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include<fstream>
#include "FontTexture.h"
#include<string.h>
#include "Log.h"
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

    float font_height = 16.0f;
    int result = stbtt_BakeFontBitmap(ttf_buffer, 0, font_height, 
                                      bitmap_pixels, atlas_w, atlas_h, 
                                      32, NUM_CHARS, charData);
    
    if (result <= 0) {
        Log::err(TAG, "stbtt_BakeFontBitmap failed. does the font fit into the atlas matrix?");
    } else {
		//stbi_write_png("assets/font_dev_small.png", atlas_w, atlas_h, 1, bitmap_pixels, atlas_w);
		updateCharData();
		//flipBitmap(bitmap_pixels, atlas_w, atlas_h);
		initTexture(bitmap_pixels, GL_RED, pixelType, atlas_w, atlas_h);
	}
	delete[] ttf_buffer;
	delete[] bitmap_pixels;
}

std::vector<Vertex> FontTexture::generateVertices(const std::string& text, int x, int y, int w) {
	std::vector<Vertex> vertices;
	// current x and y of the BOTTOM LEFT of the character
	int currX = x;
	int currY = y + maxCharHeight;
	for (unsigned long i = 0; i < text.size(); i++) {
		stbtt_bakedchar c = charData[text[i] - ' '];
		int width = c.x1 - c.x0;
		int height = c.y1 - c.y0;

		Log::log(TAG, Log::oss(text[i], " c: ", text[i] - ' ', " width: ", width, " height: ", height));

		if (currX + width > x + w) {
			currX = x;
			currY += maxCharHeight;
		}

		Vertex topLeft = {
			glm::vec3(currX, currY + height, 0.0f),
			glm::vec3(0.0f),
			glm::vec3(0.0f),
			glm::vec2((float)c.x0 / ATLAS_WIDTH, (float)c.y0 / ATLAS_HEIGHT)
		};
		Vertex topRight = {
			glm::vec3(currX + width, currY + height, 0.0f),
			glm::vec3(0.0f),
			glm::vec3(0.0f),
			glm::vec2((float)c.x1 / ATLAS_WIDTH, (float)c.y0 / ATLAS_HEIGHT)
		};
		Vertex botLeft = {
			glm::vec3(currX, currY, 0.0f),
			glm::vec3(0.0f),
			glm::vec3(0.0f),
			glm::vec2((float)c.x0 / ATLAS_WIDTH, (float)c.y1 / ATLAS_HEIGHT)
		};
		Vertex botRight = {
			glm::vec3(currX + width, currY, 0.0f),
			glm::vec3(0.0f),
			glm::vec3(0.0f),
			glm::vec2((float)c.x1 / ATLAS_WIDTH, (float)c.y1 / ATLAS_HEIGHT)
		};

		vertices.insert(vertices.end(), {topLeft, topRight, botLeft, botRight});

		currX += width + 1; // leave one pixel between characters
		Log::log(TAG, Log::oss("\t topLeft: ", topLeft.to_string(), " topRight: ", topRight.to_string(), " botLeft: ", botLeft.to_string(), " botRight: ", botRight.to_string()));
		Log::log(TAG, Log::oss("\tcurrX: ", currX, " currY: ", currY));
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

void FontTexture::updateCharData() {
	stbtt_bakedchar* tallestChar = std::max_element(charData, charData + NUM_CHARS, 
		[](stbtt_bakedchar a, stbtt_bakedchar b) {
			return (a.y1 - a.y0) < (b.y1 - b.y0);
		}
	);
	maxCharHeight = tallestChar->y1 - tallestChar->y0;
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
