#include<cmath>
#include<stb/stb_image.h>
#include"ImageTexture.h"
#include"Log.h"
#include<fmt/format.h>

ImageTexture::ImageTexture(const char* image, TextureType texType, GLenum pixelType, bool convertToSpecular) {
	type = texType;

	int widthImg, heightImg, numColCh;
	stbi_set_flip_vertically_on_load(true);

	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);
	Log::log(TAG, fmt::format("loaded image; colour channels: {}", numColCh));

	if (texType == TextureType::Specular && numColCh >= 3) {
		unsigned char* redChannel = new unsigned char[widthImg * heightImg];
		for (int i = 0; i < widthImg * heightImg; i++) {
			unsigned char r = bytes[i * numColCh + 0];
			unsigned char g = bytes[i * numColCh + 1];
			unsigned char b = bytes[i * numColCh + 2];

			float gray = r; // image is black and white if texType == specular and convertToSpecular == false

			if (convertToSpecular) {
				gray = R_COEFFICIENT * r + G_COEFFICIENT * g + B_COEFFICIENT * b;

				gray = pow(gray / 255.0f, GAMMA_CORRECT) * 255.0f;
				gray = std::min(gray * BOOST_COEFFICIENT, 255.0f);
			}

			redChannel[i] = static_cast<unsigned char>(gray);
		}

		initTexture(redChannel, GL_RED, pixelType, widthImg, heightImg);
		delete[] redChannel;
	} else if (texType == TextureType::Diffuse) {
		int colorFormat = 0;
		switch (numColCh) {
			case 4: colorFormat = GL_RGBA; break;
			case 3: colorFormat = GL_RGB; break;
			case 2: colorFormat = GL_RG; break;
			case 1: colorFormat = GL_RED; break;
			default: Log::err(TAG, fmt::format("number of colour channels = {} not supported!", numColCh));
		}
		if (colorFormat) initTexture(bytes, colorFormat, pixelType, widthImg, heightImg);
	} else {
		Log::err(TAG, "sorry i didn't implement specular using number of colour channels less than 3 yet lol");
	}

	stbi_image_free(bytes);
}