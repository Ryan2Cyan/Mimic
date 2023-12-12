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

		/// <summary>
		/// Bind render texture for drawing.
		/// </summary>
		void Bind() const noexcept;

		/// <summary>
		/// Unbind render texture after drawing.
		/// </summary>
		void Unbind() const noexcept;

		/// <summary>
		/// Sets up the stored texture within the render texture to be drawn to. The viewport is also
		/// adjusted to match the stored texture's aspect ratio.
		/// </summary>
		void AttachTexture(const TextureTarget& textureTarget, const std::uint8_t& params, const RenderTextureAttachment& attachment = RenderTextureAttachment::MIMIC_COLOR, const int level = 0);

		/// <summary>
		/// Sets up the render buffer object within the render texture to be drawn to. The viewport is 
		/// also adjusted to match the render buffer object's aspect ratio.
		/// </summary>
		void UseRenderObject(const glm::ivec2& aspectRatio) const;

		/// <summary>
		/// Set render texture's stored texture.
		/// </summary>
		void SetTexture(const std::shared_ptr<Texture>& texture);

		/// <summary>
		/// Change viewport to match the stored texture's aspect ratio.
		/// </summary>
		void SetTextureViewPort() const noexcept;

		/// <summary>
		/// Get stored texture ID.
		/// </summary>
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