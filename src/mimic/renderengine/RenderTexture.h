#pragma once
#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <renderengine/Texture.h>
#include <memory>
#include <cstdint>

namespace MimicRender
{
	// #############################################################################
	// framebuffer stuct:
	// #############################################################################
	struct Texture;

	enum class RenderTextureAttachment
	{
		MIMIC_COLOR,
		MIMIC_DEPTH
	};

	struct RenderTexture
	{
		static std::shared_ptr<RenderTexture> Initialise() noexcept;

		void Bind() const noexcept;
		void Unbind() const noexcept;
		void AttachTexture(const TextureTarget& textureTarget, const std::uint8_t& params, const RenderTextureAttachment& attachment = RenderTextureAttachment::MIMIC_COLOR, const int level = 0);
		void UseRenderObject(const glm::ivec2& aspectRatio) const;
		void SetTexture(const std::shared_ptr<Texture>& texture);
		void SetTextureViewPort() const noexcept;
		const unsigned int GetTextureID() const;

		// render texture parameters:
		static const std::uint8_t MIMIC_NO_DRAW			 = 0b1;
		static const std::uint8_t MIMIC_NO_READ			 = 0b10;
		static const std::uint8_t MIMIC_COLOR_BUFFER_BIT = 0b100;
		static const std::uint8_t MIMIC_DEPTH_BUFFER_BIT = 0b1000;
		static const std::uint8_t MIMIC_DEPTH_AND_COLOR  = 0b1'0000;
		static const std::uint8_t MIMIC_VIEWPORT	     = 0b10'0000;

	private:
		friend struct Renderer;
		friend struct EnvironmentCubeMap;

		std::shared_ptr<Texture> _texture;
		unsigned int _id = 0;
		unsigned int _depthRBOId;
		bool _initialised;
	};
}