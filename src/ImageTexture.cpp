#include<cmath>
#include<stb/stb_image.h>
#include"ImageTexture.h"
#include"Log.h"

ImageTexture::ImageTexture(const char* image, TextureType texType, GLenum pixelType) {
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

		initTexture(redChannel, GL_RED, pixelType, widthImg, heightImg);
		delete[] redChannel;
	} else {
		if (type == TextureType::Specular) {
			initTexture(bytes, GL_RED, pixelType, widthImg, heightImg);
		} else {
			initTexture(bytes, GL_RGBA, pixelType, widthImg, heightImg);
		}
	}
	// Deletes the image data as it is already in the OpenGL Texture object
	stbi_image_free(bytes);
}