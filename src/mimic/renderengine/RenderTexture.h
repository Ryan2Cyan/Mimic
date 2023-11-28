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
		void BindTextureForRender(const TextureTarget& textureTarget, const std::uint8_t& params, const int level = 0, const RenderTextureAttachment& attachment = RenderTextureAttachment::MIMIC_COLOR);
		void UseRenderObject(const glm::ivec2& aspectRatio) const;
		void SetTexture(const std::shared_ptr<Texture>& texture);
		const unsigned int GetTextureID() const;

		// render texture parameters:
		static const std::uint8_t MIMIC_NO_DRAW			 = 0b1;
		static const std::uint8_t MIMIC_NO_READ			 = 0b10;
		static const std::uint8_t MIMIC_COLOR_BUFFER_BIT = 0b100;
		static const std::uint8_t MIMIC_DEPTH_BUFFER_BIT = 0b1000;

	private:
		friend struct Renderer;
		friend struct EnvironmentCubeMap;

		std::shared_ptr<Texture> _texture;
		unsigned int _id = 0;
		unsigned int _depthRBOId;
		bool _initialised;
	};
}