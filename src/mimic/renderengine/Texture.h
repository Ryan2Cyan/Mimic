#pragma once
#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <string>
#include <cstdint>
#include <vector>
#include <memory>

namespace MimicRender
{
	enum class TextureType
	{
		MIMIC_DIFFUSE,
		MIMIC_SPECULAR,
		MIMIC_NORMAL,
		MIMIC_HEIGHT,
		MIMIC_ALBEDO,
		MIMIC_ROUGHNESS,
		MIMIC_METALLIC,
		MIMIC_HDRCUBEMAP,
		MIMIC_NO_TYPE
	};

	enum class TextureTarget
	{
		MIMIC_TEXTURE_2D = 0,
		MIMIC_CUBE_MAP_POSITIVE_X = 1,
		MIMIC_CUBE_MAP_NEGATIVE_X = 2,
		MIMIC_CUBE_MAP_POSITIVE_Y = 3,
		MIMIC_CUBE_MAP_NEGATIVE_Y = 4,
		MIMIC_CUBE_MAP_POSITIVE_Z = 5,
		MIMIC_CUBE_MAP_NEGATIVE_Z = 6
	};

	// #############################################################################
	// texture stuct:
	// #############################################################################
	// Source: https://www.learncpp.com/cpp-tutorial/bit-manipulation-with-bitwise-operators-and-bit-masks/
	// Source: https://cplusplus.com/forum/general/1590/
	struct Texture
	{
	private:
		friend struct Renderer;
		friend struct Mesh;
		friend struct Model;
		friend struct RenderTexture;
		friend struct EnvironmentCubeMap;

		// texture type
		static const std::uint16_t MIMIC_2D_TEXTURE =		 0b0000'0000'0000'0001;
		static const std::uint16_t MIMIC_CUBEMAP_TEXTURE =	 0b0000'0000'0000'0010;

		// texture parameters:
		static const std::uint16_t MIMIC_UNSIGNED_BYTE =	 0b0000'0000'0000'0100;
		static const std::uint16_t MIMIC_FLOAT =			 0b0000'0000'0000'1000;

		static const std::uint16_t MIMIC_WRAPT_REPEAT =		 0b0000'0000'0001'0000;
		static const std::uint16_t MIMIC_WRAPS_REPEAT =		 0b0000'0000'0010'0000;
		static const std::uint16_t MIMIC_WRAPT_CLAMP =		 0b0000'0000'0100'0000;
		static const std::uint16_t MIMIC_WRAPS_CLAMP =		 0b0000'0000'1000'0000;
		static const std::uint16_t MIMIC_WRAPR_CLAMP =		 0b0000'0001'0000'0000;
		
		static const std::uint16_t MIMIC_MIN_LINEAR =		 0b0000'0010'0000'0000;
		static const std::uint16_t MIMIC_MAG_LINEAR =		 0b0000'0100'0000'0000;
		static const std::uint16_t MIMIC_MIN_MIPMAP_LINEAR = 0b0000'1000'0000'0000;
		static const std::uint16_t MIMIC_MAG_MIPMAP_LINEAR = 0b0001'0000'0000'0000;
		static const std::uint16_t MIMIC_GEN_MIPMAP =		 0b0010'0000'0000'0000;

		// engine texture parameters:
		static const std::uint16_t MIMIC_BRDF_TEXTURE_PARAMS = MIMIC_2D_TEXTURE | MIMIC_FLOAT | MIMIC_WRAPS_CLAMP | MIMIC_WRAPT_CLAMP | MIMIC_MIN_LINEAR | MIMIC_MAG_LINEAR;
		static const std::uint16_t MIMIC_CUBEMAP_TEXTURE_PARAMS = MIMIC_CUBEMAP_TEXTURE | MIMIC_FLOAT | MIMIC_WRAPS_CLAMP | MIMIC_WRAPT_CLAMP | MIMIC_WRAPR_CLAMP | MIMIC_MIN_LINEAR | MIMIC_MAG_LINEAR;
		
		unsigned int _id;
		TextureType _type;

		static const GLenum GetGLTarget(const std::uint16_t& textureParams) noexcept;
		static const GLenum GetGLDataType(const std::uint16_t& textureParams) noexcept;
		static void GLTextureParams(const std::uint16_t& textureParams, const GLenum& target) noexcept;
		static const bool GLSendData(const GLenum& target, const GLint& internalFormat, const glm::ivec2& aspectRatio, const GLenum& format, const GLenum& dataType, const unsigned char* data) noexcept;

	public:
		enum TextureFormats
		{
			MIMIC_RGB = 0x01,
			MIMIC_RGBA = 0x02,
			MIMIC_RGB16F = 0x04,
			MIMIC_RED = 0x08,
			MIMIC_RG = 0x10,
			MIMIC_RG16F = 0x20
		};

		static const std::shared_ptr<Texture> Initialise(const std::string& fullPath, const glm::ivec2& aspectRatio, const std::uint16_t& textureParams, const TextureType& type = TextureType::MIMIC_NO_TYPE);
		static const std::shared_ptr<Texture> Initialise(const glm::ivec2& aspectRatio, const std::uint16_t& textureParams, const TextureFormats& internalFormat, const TextureFormats& format, const TextureType& textureType = TextureType::MIMIC_NO_TYPE);
		void SetType(const TextureType& textureType);
		const unsigned int GetId() const noexcept;

		// user texture parameters:
		static const std::uint16_t MIMIC_2D_TEXTURE_PARAMS = MIMIC_2D_TEXTURE | MIMIC_UNSIGNED_BYTE | MIMIC_WRAPS_REPEAT | MIMIC_WRAPT_REPEAT | MIMIC_MIN_MIPMAP_LINEAR | MIMIC_MAG_LINEAR | MIMIC_GEN_MIPMAP;
	};
}