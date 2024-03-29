#include "Texture.h"
# include <mimic_utility/Logger.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ASSERT(x)
#include <stb_image.h>

namespace MimicRender
{
	// #############################################################################
	// Texture functions:
	// #############################################################################
	std::shared_ptr<Texture> Texture::Initialise(const std::string& fullPath, const glm::ivec2& aspectRatio, const std::uint32_t& textureParams, const TextureType& textureType)
	{
		// Generate texture ID:
		unsigned int textureId;
		glGenTextures(1, &textureId);

		// Have the stbi library load texture data from file path:
		int width;
		int height;
		int componentsN;
		if(textureParams & MIMIC_FLIP_VERTICAL) stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(fullPath.c_str(), &width, &height, &componentsN, 0);
		if (data == nullptr)
		{
			MIMIC_LOG_WARNING("[MimicRender::Texture] Texture data could not be loaded by stbi, at path: \"%\"", fullPath);
			stbi_image_free(data);
			return nullptr;
		}

		// Decide format from stbi load:
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
				MIMIC_LOG_WARNING("[MimicRender::Texture] No pixel data format was found at path: \"%\"", fullPath);
				stbi_image_free(data);
			}break;
		}

		// Load texture parameters:
		const GLenum target = GetGLTarget(textureParams);
		if (target == 0) return nullptr;
		const GLenum dataType = GetGLDataType(textureParams);
		if (dataType == 0) return nullptr;

		// Create texture & send it to the GPU:
		glBindTexture(target, textureId);
		glTexImage2D(target, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		GLTextureParams(textureParams, target);
		stbi_image_free(data);
		stbi_set_flip_vertically_on_load(false);

		std::shared_ptr<Texture> texture = std::make_shared<Texture>();
		texture->_id = textureId;
		texture->_type = textureType;
		texture->_aspectRatio = aspectRatio;
		texture->_initialised = true;
		return texture;
	}

	std::shared_ptr<Texture> Texture::Initialise(const glm::ivec2& aspectRatio, const std::uint32_t& textureParams, const TextureFormats& internalFormat, const TextureFormats& format, const TextureType& textureType)
	{
		// Load texture parameters:
		const GLenum target = GetGLTarget(textureParams);
		if (target == 0) return nullptr;
		const GLenum dataType = GetGLDataType(textureParams);
		if (dataType == 0) return nullptr;

		// Decide internal format:
		GLint internalFormatGL = 0;
		if (internalFormat & TextureFormats::MIMIC_RGB) internalFormatGL = GL_RGB;
		else if (internalFormat & TextureFormats::MIMIC_RGBA) internalFormatGL = GL_RGBA;
		else if (internalFormat & TextureFormats::MIMIC_RGB16F) internalFormatGL = GL_RGB16F;
		else if (internalFormat & TextureFormats::MIMIC_RED) internalFormatGL = GL_RED;
		else if (internalFormat & TextureFormats::MIMIC_RG) internalFormatGL = GL_RG;
		else if (internalFormat & TextureFormats::MIMIC_RG16F) internalFormatGL = GL_RG16F;
		else if (internalFormat & TextureFormats::MIMIC_DEPTH_COMPONENT) internalFormatGL = GL_DEPTH_COMPONENT;
		else
		{
			MIMIC_LOG_WARNING("[MimicRender::Texture] Could not create texture, no valid texture internal format arguement.");
			MIMIC_LOG_OPENGL("Texture");
			return nullptr;
		}

		// Decide format:
		GLenum formatGL = 0;
		if (format & TextureFormats::MIMIC_RGB) formatGL = GL_RGB;
		else if (format & TextureFormats::MIMIC_RGBA) formatGL = GL_RGBA;
		else if (format & TextureFormats::MIMIC_RGB16F) formatGL = GL_RGB16F; 
		else if (format & TextureFormats::MIMIC_RED) formatGL = GL_RED;
		else if (format & TextureFormats::MIMIC_RG) formatGL = GL_RG;
		else if (format & TextureFormats::MIMIC_RG16F) formatGL = GL_RG16F;
		else if (format & TextureFormats::MIMIC_DEPTH_COMPONENT) formatGL = GL_DEPTH_COMPONENT;
		else
		{
			MIMIC_LOG_WARNING("[MimicRender::Texture] Could not create texture, no valid texture format arguement.");
			MIMIC_LOG_OPENGL("Texture");
			return nullptr;
		}

		// Generate texture ID:
		unsigned int textureId;
		glGenTextures(1, &textureId);
		glBindTexture(target, textureId);

		// create texture & send it to the GPU:
		if (textureParams & MIMIC_2D_TEXTURE)
		{
			glTexImage2D(target, 0, internalFormatGL, aspectRatio.x, aspectRatio.y, 0, formatGL, dataType, nullptr);
			
		}
		else if (textureParams & MIMIC_CUBEMAP_TEXTURE)
		{
			for (unsigned int i = 0; i < 6; i++)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormatGL, aspectRatio.x, aspectRatio.y, 0, formatGL, dataType, nullptr);
			}
		}
		
		GLTextureParams(textureParams, target);

		glBindTexture(target, 0);

		std::shared_ptr<Texture> texture = std::make_shared<Texture>();
		texture->_id = textureId;
		texture->_type = textureType;
		texture->_aspectRatio = aspectRatio;
		texture->_initialised = true;
		return texture;
	}

	const GLenum Texture::GetGLTarget(const std::uint32_t& textureParams) noexcept
	{
		if (textureParams & MIMIC_2D_TEXTURE) return GL_TEXTURE_2D;
		else if (textureParams & MIMIC_CUBEMAP_TEXTURE) return GL_TEXTURE_CUBE_MAP;
		else
		{
			MIMIC_LOG_WARNING("[MimicRender::Texture] Could not create texture, no valid OpenGL target arguement.");
			MIMIC_LOG_OPENGL("Texture");
			return 0;
		}
	}

	const GLenum Texture::GetGLDataType(const std::uint32_t& textureParams) noexcept
	{
		if (textureParams & MIMIC_UNSIGNED_BYTE) return GL_UNSIGNED_BYTE;
		else if (textureParams & MIMIC_FLOAT) return GL_FLOAT;
		else
		{
			MIMIC_LOG_WARNING("[MimicRender::Texture] Could not create texture, no valid OpenGL data type arguement.");
			MIMIC_LOG_OPENGL("Texture");
			return 0;
		}
	}

	void Texture::GLTextureParams(const std::uint32_t& textureParams, const GLenum& target) noexcept
	{
		if (textureParams & MIMIC_WRAPS_REPEAT) glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
		if (textureParams & MIMIC_WRAPT_REPEAT) glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);

		if (textureParams & MIMIC_WRAPS_CLAMP_TO_BORDER) glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		if (textureParams & MIMIC_WRAPT_CLAMP_TO_BORDER) glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		if (textureParams & MIMIC_WRAPS_CLAMP) glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		if (textureParams & MIMIC_WRAPT_CLAMP) glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		if (textureParams & MIMIC_WRAPR_CLAMP) glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		if (textureParams & MIMIC_MIN_LINEAR) glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		if (textureParams & MIMIC_MAG_LINEAR) glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (textureParams & MIMIC_MIN_NEAREST) glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		if (textureParams & MIMIC_MAG_NEAREST) glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		if (textureParams & MIMIC_MIN_MIPMAP_LINEAR) glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		if (textureParams & MIMIC_MAG_MIPMAP_LINEAR) glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		if (textureParams & MIMIC_CLAMP_BORDER_COLOR)
		{
			constexpr float borderColour[] = { 1.0f, 1.0f, 1.0f, 1.0f };
			glTexParameterfv(target, GL_TEXTURE_BORDER_COLOR, borderColour);
		}

		if (textureParams & MIMIC_GEN_MIPMAP) glGenerateMipmap(target);
	}

	void Texture::SetType(const TextureType& textureType)
	{
		if (!_initialised) return;
		_type = textureType;
	}

	const unsigned int Texture::GetId() const noexcept
	{
		if (!_initialised) return 0;
		return _id;
	}

	const glm::ivec2 Texture::GetAspectRatio() const noexcept
	{
		if (!_initialised) return glm::ivec2(0.0f);
		return _aspectRatio;
	}
}