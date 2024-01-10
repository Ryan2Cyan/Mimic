#pragma once
#include <string>
#include <memory>
#include <GLM/glm.hpp>
#include <GL/glew.h>

namespace MimicRender
{
	/// <summary>
	/// General summary of a textures application type to a mesh/model.
	/// </summary>
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

	/// <summary>
	/// Determines what type of texture certain OpenGL functions modify or allocate.
	/// </summary>
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

	// Source: https://www.learncpp.com/cpp-tutorial/bit-manipulation-with-bitwise-operators-and-bit-masks/
	// Source: https://cplusplus.com/forum/general/1590/
	
	/// <summary>
	/// Loads 2D textures and stores their ID for rendering access.
	/// </summary>
	struct Texture
	{
		// Texture types:
		static const std::uint32_t MIMIC_2D_TEXTURE = 0b1;
		static const std::uint32_t MIMIC_CUBEMAP_TEXTURE = 0b10;

	private:
		friend struct Renderer;
		friend struct Mesh;
		friend struct Model;
		friend struct RenderTexture;
		friend struct EnvironmentCubeMap;
		friend struct Shader;

		// Texture parameters:
		static const std::uint32_t MIMIC_UNSIGNED_BYTE =				 0b100;
		static const std::uint32_t MIMIC_FLOAT =						 0b1000;

		static const std::uint32_t MIMIC_WRAPT_REPEAT =					 0b1'000;
		static const std::uint32_t MIMIC_WRAPS_REPEAT =					 0b10'0000;
		static const std::uint32_t MIMIC_WRAPT_CLAMP_TO_BORDER =		 0b10'000;
		static const std::uint32_t MIMIC_WRAPS_CLAMP_TO_BORDER =		 0b100'0000;
		static const std::uint32_t MIMIC_WRAPT_CLAMP =				     0b1000'0000;
		static const std::uint32_t MIMIC_WRAPS_CLAMP =					 0b1'0000'0000;
		static const std::uint32_t MIMIC_WRAPR_CLAMP =					 0b10'0000'0000;
		
		static const std::uint32_t MIMIC_MIN_LINEAR =					 0b100'0000'0000;
		static const std::uint32_t MIMIC_MAG_LINEAR =					 0b1000'0000'0000;
		static const std::uint32_t MIMIC_MIN_NEAREST =					 0b1'0000'0000'0000;
		static const std::uint32_t MIMIC_MAG_NEAREST =					 0b10'0000'0000'0000;
		static const std::uint32_t MIMIC_MIN_MIPMAP_LINEAR =			 0b100'0000'0000'0000;
		static const std::uint32_t MIMIC_MAG_MIPMAP_LINEAR =			 0b1000'0000'0000'0000;
		static const std::uint32_t MIMIC_GEN_MIPMAP =					 0b1'0000'0000'0000'0000;
		static const std::uint32_t MIMIC_FLIP_VERTICAL =				 0b10'0000'0000'0000'0000;
		static const std::uint32_t MIMIC_CLAMP_BORDER_COLOR =			 0b100'0000'0000'0000'0000;

		// engine texture parameters:
		static const std::uint32_t MIMIC_BRDF_TEXTURE_PARAMS = MIMIC_2D_TEXTURE | MIMIC_FLOAT | MIMIC_WRAPS_CLAMP | MIMIC_WRAPT_CLAMP | MIMIC_MIN_LINEAR | MIMIC_MAG_LINEAR;
		static const std::uint32_t MIMIC_CUBEMAP_TEXTURE_PARAMS = MIMIC_CUBEMAP_TEXTURE | MIMIC_FLOAT | MIMIC_WRAPS_CLAMP | MIMIC_WRAPT_CLAMP | MIMIC_WRAPR_CLAMP | MIMIC_MIN_LINEAR | MIMIC_MAG_LINEAR;
		static const std::uint32_t MIMIC_PREFILTERED_CUBEMAP_TEXTURE_PARAMS = MIMIC_CUBEMAP_TEXTURE | MIMIC_FLOAT | MIMIC_WRAPS_CLAMP | MIMIC_WRAPT_CLAMP | MIMIC_WRAPR_CLAMP | MIMIC_MIN_MIPMAP_LINEAR | MIMIC_MAG_LINEAR | MIMIC_GEN_MIPMAP;
		
		glm::ivec2 _aspectRatio;
		TextureType _type;
		unsigned int _id;
		bool _initialised;

		static const GLenum GetGLTarget(const std::uint32_t& textureParams) noexcept;
		static const GLenum GetGLDataType(const std::uint32_t& textureParams) noexcept;
		static void GLTextureParams(const std::uint32_t& textureParams, const GLenum& target) noexcept;

	public:
		enum TextureFormats
		{
			MIMIC_RGB = 0x01,
			MIMIC_RGBA = 0x02,
			MIMIC_RGB16F = 0x04,
			MIMIC_RED = 0x08,
			MIMIC_RG = 0x10,
			MIMIC_RG16F = 0x20,
			MIMIC_DEPTH_COMPONENT = 0x40
		};

		static std::shared_ptr<Texture> Initialise(const std::string& fullPath, const glm::ivec2& aspectRatio = glm::ivec2(0.0f), const std::uint32_t& textureParams = MIMIC_2D_TEXTURE_PARAMS, const TextureType& textureType = TextureType::MIMIC_NO_TYPE);
		static std::shared_ptr<Texture> Initialise(const glm::ivec2& aspectRatio, const std::uint32_t& textureParams, const TextureFormats& internalFormat, const TextureFormats& format, const TextureType& textureType = TextureType::MIMIC_NO_TYPE);

		/// <summary>
		/// Set the textures's type, used when materials need to interpret what texture it's looking at.
		/// </summary>
		/// <param name="textureType">How the texture will be used when rendering a mesh.</param>
		void SetType(const TextureType& textureType);

		const glm::ivec2 GetAspectRatio() const noexcept;
		const unsigned int GetId() const noexcept;

		// user texture parameters:
		static const std::uint32_t MIMIC_2D_TEXTURE_PARAMS = MIMIC_2D_TEXTURE | MIMIC_UNSIGNED_BYTE | MIMIC_WRAPS_REPEAT | MIMIC_WRAPT_REPEAT | MIMIC_MIN_MIPMAP_LINEAR | MIMIC_MAG_LINEAR | MIMIC_GEN_MIPMAP;
		static const std::uint32_t MIMIC_DEPTH_MAP_PARAMS = MIMIC_2D_TEXTURE | MIMIC_FLOAT | MIMIC_WRAPS_CLAMP_TO_BORDER | MIMIC_WRAPT_CLAMP_TO_BORDER | MIMIC_MIN_NEAREST | MIMIC_MAG_NEAREST | MIMIC_CLAMP_BORDER_COLOR; // NOTE: make private after implementing shadow map class
	};
}