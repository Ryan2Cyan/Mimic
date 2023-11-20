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

	const int Texture::Create(const glm::ivec2& aspectRatio, const std::uint16_t& textureParams, const TextureFormats& internalFormat, const TextureFormats& format)
	{
		// decide target:
		GLenum target = 0;
		if (textureParams & MIMIC_2D_TEXTURE) target = GL_TEXTURE_2D;
		else if (textureParams & MIMIC_CUBEMAP_TEXTURE) target = GL_TEXTURE_CUBE_MAP;
		else
		{
			MIMIC_LOG_WARNING("[Mimic::Texture] Could not create texture, no valid texture type arguement.");
			MIMIC_LOG_OPENGL("Texture");
			return -1;
		}

		// decide type:
		GLenum type = 0;
		if (textureParams & MIMIC_UNSIGNED_BYTE) type = GL_UNSIGNED_BYTE;
		else if (textureParams & MIMIC_FLOAT) type = GL_FLOAT;
		else
		{
			MIMIC_LOG_WARNING("[Mimic::Texture] Could not create texture, no valid texture type arguement.");
			MIMIC_LOG_OPENGL("Texture");
			return -1;
		}

		// decide internal format:
		GLint internalFormatGL = 0;
		if (internalFormat & TextureFormats::MIMIC_RGB) internalFormatGL = GL_RGB;
		else if (internalFormat & TextureFormats::MIMIC_RGBA) internalFormatGL = GL_RGBA;
		else if (internalFormat & TextureFormats::MIMIC_RGB16F) internalFormatGL = GL_RGB16F;
		else if (internalFormat & TextureFormats::MIMIC_RED) internalFormatGL = GL_RED;
		else if (internalFormat & TextureFormats::MIMIC_RG) internalFormatGL = GL_RG;
		else if (internalFormat & TextureFormats::MIMIC_RG16F) internalFormatGL = GL_RG16F;
		else
		{
			MIMIC_LOG_WARNING("[Mimic::Texture] Could not create texture, no valid texture internal format arguement.");
			MIMIC_LOG_OPENGL("Texture");
			return -1;
		}

		// decide format:
		GLenum formatGL = 0;
		if (format & TextureFormats::MIMIC_RGB) formatGL = GL_RGB;
		else if (format & TextureFormats::MIMIC_RGBA) formatGL = GL_RGBA;
		else if (format & TextureFormats::MIMIC_RGB16F) formatGL = GL_RGB16F; 
		else if (format & TextureFormats::MIMIC_RED) formatGL = GL_RED;
		else if (format & TextureFormats::MIMIC_RG) formatGL = GL_RG;
		else if (format & TextureFormats::MIMIC_RG16F) formatGL = GL_RG16F;
		else
		{
			MIMIC_LOG_WARNING("[Mimic::Texture] Could not create texture, no valid texture format arguement.");
			MIMIC_LOG_OPENGL("Texture");
			return -1;
		}


		glGenTextures(1, &_id);
		glBindTexture(target, _id);

		if (textureParams & MIMIC_2D_TEXTURE)
		{
			glTexImage2D(target, 0, internalFormatGL, aspectRatio.x, aspectRatio.y, 0, formatGL, type, nullptr);
		}
		else if (textureParams & MIMIC_CUBEMAP_TEXTURE)
		{
			for (unsigned int i = 0; i < 6; i++)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormatGL, aspectRatio.x, aspectRatio.y, 0, formatGL, type, nullptr);
			}
		}
		else
		{
			MIMIC_LOG_WARNING("[Mimic::Texture] Could not create texture, no valid texture target arguement.");
			MIMIC_LOG_OPENGL("Texture");
			return -1;
		}
		
		if (textureParams & MIMIC_WRAPS_REPEAT) glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
		if (textureParams & MIMIC_WRAPT_REPEAT) glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);

		if (textureParams & MIMIC_WRAPS_CLAMP) glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		if (textureParams & MIMIC_WRAPT_CLAMP) glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		if (textureParams & MIMIC_WRAPR_CLAMP) glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		if (textureParams & MIMIC_MIN_LINEAR) glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		if (textureParams & MIMIC_MAG_LINEAR) glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (textureParams & MIMIC_MIN_MIPMAP_LINEAR) glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		if (textureParams & MIMIC_MAG_MIPMAP_LINEAR) glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		if (textureParams & MIMIC_GEN_MIPMAP) glGenerateMipmap(target); 

		glBindTexture(target, 0);
		return 0;
	}

	void Texture::SetType(const int& type)
	{
		_type = type;
	}
}