#define STB_TRUETYPE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include<fstream>
#include "FontTexture.h"
#include<string.h>
#include "Log.h"
#include"stb/stb_image_write.h"
#include"stb/stb_truetype.h"

FontTexture::FontTexture(const char* ttfFile, GLuint slot, GLenum pixelType) {
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

	int atlas_w = 96;
    int atlas_h = 96;
	
    unsigned char* bitmap_pixels = new unsigned char[atlas_w * atlas_h];
    stbtt_bakedchar cdata[96]; // Allocation for ASCII 32-128

    float font_height = 16.0f;
    int result = stbtt_BakeFontBitmap(ttf_buffer, 0, font_height, 
                                      bitmap_pixels, atlas_w, atlas_h, 
                                      32, 96, cdata);
    
    if (result <= 0) {
        Log::err(TAG, "stbtt_BakeFontBitmap failed. does the font fit into the atlas matrix?");
    } else {
		//stbi_write_png("assets/font_dev_small.png", atlas_w, atlas_h, 1, bitmap_pixels, atlas_w);
		flipBitmap(bitmap_pixels, atlas_w, atlas_h);
		initTexture(bitmap_pixels, slot, GL_RED, pixelType, atlas_w, atlas_h);
	}
	delete[] ttf_buffer;
	delete[] bitmap_pixels;
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
