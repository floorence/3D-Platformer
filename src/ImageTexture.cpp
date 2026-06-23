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
	Log::log(TAG, "loaded image");
	Log::log(TAG, fmt::format("colour channels: {}", numColCh));

	if (convertToSpecular && numColCh >= 3) {
		type = TextureType::Specular;

		unsigned char* redChannel = new unsigned char[widthImg * heightImg];
		for (int i = 0; i < widthImg * heightImg; i++) {
			unsigned char r = bytes[i * numColCh + 0];
			unsigned char g = bytes[i * numColCh + 1];
			unsigned char b = bytes[i * numColCh + 2];
			float gray = R_COEFFICIENT * r + G_COEFFICIENT * g + B_COEFFICIENT * b;

			gray = pow(gray / 255.0f, GAMMA_CORRECT) * 255.0f;
			gray = std::min(gray * BOOST_COEFFICIENT, 255.0f);

			redChannel[i] = static_cast<unsigned char>(gray);
		}

		initTexture(redChannel, GL_RED, pixelType, widthImg, heightImg);
		delete[] redChannel;
	} else if (convertToSpecular) {
		Log::err(TAG, "cannot convert to specular image with less than 3 colour channels!");
	} else {
		if (texType == TextureType::Specular) {
			initTexture(bytes, GL_RED, pixelType, widthImg, heightImg);
		} else {
			initTexture(bytes, GL_RGBA, pixelType, widthImg, heightImg);
		}
	}

	stbi_image_free(bytes);
}