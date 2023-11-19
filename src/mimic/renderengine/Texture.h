#pragma once
#include <lowlevelsystems/Resource.h>
#include <string>
#include <GL/glew.h>
#include <cstdint>
#include <GLM/glm.hpp>

namespace Mimic
{
	// #############################################################################
	// texture stuct:
	// #############################################################################
	// Source: https://www.learncpp.com/cpp-tutorial/bit-manipulation-with-bitwise-operators-and-bit-masks/
	// Source: https://cplusplus.com/forum/general/1590/
	struct Texture : Resource
	{
	private:
		friend struct ModelRenderer;
		friend struct Renderer;
		friend struct Mesh;
		friend struct Model;
		friend struct Material;
		friend struct EnvironmentCubeMap;
		friend struct BasicMaterial;
		friend struct PBRMaterial;

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
		std::string _type;

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

		const int Load(const std::string& path) override;
		const int Create(const glm::ivec2& aspectRatio, const std::uint16_t& textureParams, const TextureFormats& internalFormat, const TextureFormats& format);

		// user texture parameters:
		static const std::uint16_t MIMIC_MODEL_TEXTURE_PARAMS = MIMIC_2D_TEXTURE | MIMIC_UNSIGNED_BYTE | MIMIC_WRAPS_REPEAT | MIMIC_WRAPT_REPEAT | MIMIC_MIN_MIPMAP_LINEAR | MIMIC_MAG_LINEAR | MIMIC_GEN_MIPMAP;
	};
}