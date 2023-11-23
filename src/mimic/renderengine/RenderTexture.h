#pragma once
#include <renderengine/Texture.h>
#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <memory>

namespace Mimic
{
	// #############################################################################
	// framebuffer stuct:
	// #############################################################################
	struct RenderbufferObject;
	struct Texture;

	struct RenderTexture
	{
		static std::shared_ptr<RenderTexture> Initialise() noexcept;

		void Bind() const noexcept;
		void Unbind() const noexcept;
		void BindTextureForRender(const TextureTarget& textureTarget, const int level = 0);
		void UseRenderObject(const glm::ivec2& aspectRatio) const;
		void SetTexture(const std::shared_ptr<Texture>& texture);

	private:
		friend struct ResourceManager;
		friend struct EnvironmentCubeMap;
		friend struct PBRMaterial;

		std::shared_ptr<Texture> _texture;
		unsigned int _id = 0;
		unsigned int _depthRBOId;
		bool _initialised;
	};
}