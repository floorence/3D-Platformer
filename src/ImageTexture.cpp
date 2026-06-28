#include<cmath>
#include<stb/stb_image.h>
#include"ImageTexture.h"
#include"Log.h"
#include<fmt/format.h>

ImageTexture::ImageTexture(const char* image, TextureType texType, GLenum pixelType, bool convertToSpecular) {
	type = texType;

	int widthImg, heightImg, numColCh;
	stbi_set_flip_vertically_on_load(true);

	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, STBI_rgb_alpha); // force 4 colour channels
	int realNumColCh = 4;
	Log::log(TAG, fmt::format("loaded image; original colour channels: {}", numColCh));

	if (texType == TextureType::Specular) {
		unsigned char* redChannel = new unsigned char[widthImg * heightImg];
		for (int i = 0; i < widthImg * heightImg; i++) {
			unsigned char r = bytes[i * realNumColCh + 0];
			unsigned char g = bytes[i * realNumColCh + 1];
			unsigned char b = bytes[i * realNumColCh + 2];

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
	} else { // texType == diffuse
		initTexture(bytes, GL_RGBA, pixelType, widthImg, heightImg);
	}

	stbi_image_free(bytes);
}