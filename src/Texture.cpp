#define STB_TRUETYPE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include"Texture.h"
#include"Log.h"
#include<fstream>
#include<cmath>
#include"stb/stb_image_write.h"

Texture::Texture(const char* image, TextureType texType, GLuint slot, GLenum pixelType) {
	// Assigns the type of the texture ot the texture object
	type = texType;

	// Stores the width, height, and the number of color channels of the image
	int widthImg, heightImg, numColCh;
	// Flips the image so it appears right side up
	stbi_set_flip_vertically_on_load(true);
	// Reads the image from a file and stores it in bytes
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);
	Log::log(TAG, "loaded image");
	Log::log(TAG, Log::oss("colour channels: ", numColCh));

	bool specular = texType == TextureType::Specular;
	if (specular && numColCh >= 3) {
		// Create 1-channel red-only array
		unsigned char* redChannel = new unsigned char[widthImg * heightImg];
		for (int i = 0; i < widthImg * heightImg; i++) {
			unsigned char r = bytes[i * numColCh + 0];
			unsigned char g = bytes[i * numColCh + 1];
			unsigned char b = bytes[i * numColCh + 2];
			float gray = 0.2126f * r + 0.7152f * g + 0.0722f * b;

			// Apply gamma correction 
			gray = pow(gray / 255.0f, 1.0 / 2.2f) * 255.0f;

			// Apply boost to compensate for dark textures
			gray = std::min(gray * 1.5f, 255.0f);

			// Save as byte
			redChannel[i] = static_cast<unsigned char>(gray);
		}

		initTexture(redChannel, slot, GL_RED, pixelType, widthImg, heightImg);
		delete[] redChannel;
	} else {
		if (type == TextureType::Specular) {
			initTexture(bytes, slot, GL_RED, pixelType, widthImg, heightImg);
		} else {
			initTexture(bytes, slot, GL_RGBA, pixelType, widthImg, heightImg);
		}
	}
	// Deletes the image data as it is already in the OpenGL Texture object
	stbi_image_free(bytes);
}

Texture::Texture(const char* ttfFile, GLuint slot, GLenum pixelType) {
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

	int atlas_w = 192;
    int atlas_h = 192;
	
    unsigned char* bitmap_pixels = new unsigned char[atlas_w * atlas_h];
    stbtt_bakedchar cdata[96]; // Allocation for ASCII 32-128

    float font_height = 32.0f;
    int result = stbtt_BakeFontBitmap(ttf_buffer, 0, font_height, 
                                      bitmap_pixels, atlas_w, atlas_h, 
                                      32, 96, cdata);
    
    if (result <= 0) {
        Log::err(TAG, "stbtt_BakeFontBitmap failed. does the font fit into the atlas matrix?");
    } else {
		// stbi_write_png("assets/font_dev.png", atlas_w, atlas_h, 1, bitmap_pixels, atlas_w);
		flipBitmap(bitmap_pixels, atlas_w, atlas_h);
		initTexture(bitmap_pixels, slot, GL_RED, pixelType, atlas_w, atlas_h);
	}
	delete[] ttf_buffer;
	delete[] bitmap_pixels;
}

void Texture::initTexture(unsigned char* bytes, GLuint slot, GLenum format, GLenum pixelType, int width, int height) {
	glGenTextures(1, &ID);
	// Assigns the texture to a Texture Unit
	glActiveTexture(GL_TEXTURE0 + slot);
	unit = slot;
	glBindTexture(GL_TEXTURE_2D, ID);

	// Configures the type of algorithm that is used to make the image smaller or bigger
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Configures the way the texture repeats (if it does at all)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	// Assigns the image to the OpenGL Texture object
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, pixelType, bytes);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Unbinds the OpenGL Texture object so that it can't accidentally be modified
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::setTexUnit(Shader& shader, const char* uniform, GLuint unit) {
	// Gets the location of the uniform
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
	// Shader needs to be activated before changing the value of a uniform
	shader.activate();
	// Sets the value of the uniform
	glUniform1i(texUni, unit);
}

std::string Texture::typeToString(TextureType type) {
	switch (type) {
		case TextureType::Diffuse: return "diffuse";
		case TextureType::Specular: return "specular";
	}
	return "";
}

void Texture::bind() {
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::flipBitmap(unsigned char* bytes, int width, int height) {
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

Texture::~Texture() {
	glDeleteTextures(1, &ID);
}

Texture::Texture(Texture&& other) noexcept {
	type = other.type;
	ID = other.ID;
	unit = other.unit;
}

Texture& Texture::operator=(Texture&& other) noexcept {
	if (this != &other) {
		type = other.type;
		ID = other.ID;
		unit = other.unit;
	}
	return *this;
}
