#include "Texture.h"
# include <utility/Logger.h>
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ASSERT(x)
#include <stb_image.h>
#include <iostream>

namespace Mimic
{
	// #############################################################################
	// texture functions:
	// #############################################################################
	const int Texture::Load(const std::string& path)
	{
		glGenTextures(1, &_id);

		int width;
		int height;
		int componentsN;
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &componentsN, 0);
		if (data == nullptr)
		{
			MIMIC_LOG_WARNING("[Mimic::Texture] Texture data could not be loaded by stbi, at path: \"%\"", path);
			stbi_image_free(data);
			return -1;
		}

		GLenum format = GL_RGBA;
		switch (componentsN)
		{
			case 1:
			{
				format = GL_RED;
			}break;
			case 3:
			{
				format = GL_RGB;
			}break;
			case 4:
			{
				format = GL_RGBA;
			}break;
			default:
			{
				MIMIC_LOG_WARNING("[Mimic::Texture] No pixel data format was found at path: \"%\"", path);
				stbi_image_free(data);
				return -1;
			}break;
		}

		glBindTexture(GL_TEXTURE_2D, _id);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		stbi_image_free(data);
		return 0;
	}
}