#pragma once
#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <renderengine/Texture.h>
#include <memory>

namespace MimicRender
{
	// #############################################################################
	// framebuffer stuct:
	// #############################################################################
	struct Texture;

	struct RenderTexture
	{
		static std::shared_ptr<RenderTexture> Initialise() noexcept;

		void Bind() const noexcept;
		void Unbind() const noexcept;
		void BindTextureForRender(const TextureTarget& textureTarget, const int level = 0);
		void UseRenderObject(const glm::ivec2& aspectRatio) const;
		void SetTexture(const std::shared_ptr<Texture>& texture);
		const unsigned int GetTextureID() const;

	private:
		friend struct Renderer;
		friend struct EnvironmentCubeMap;

		std::shared_ptr<Texture> _texture;
		unsigned int _id = 0;
		unsigned int _depthRBOId;
		bool _initialised;
	};
}