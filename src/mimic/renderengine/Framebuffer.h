#pragma once
#include <lowlevelsystems/Resource.h>
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

	struct Framebuffer : Resource
	{
	private:
		friend struct ResourceManager;
		friend struct EnvironmentCubeMap;

		const int Create();
		const int Load(const std::string& path) override;
		void Unbind() const noexcept;
		void AttachRenderObject(const std::shared_ptr<RenderbufferObject>& renderbufferObject);
		void RenderToTexture(const unsigned int& textureId, const TextureAttachment& textureAttachment, const TextureTarget& textureTarget, const int level = 0);
		void UseRenderObject(const glm::ivec2& aspectRatio) const;

		unsigned int _id = 0;
		unsigned int _boundRenderbufferObjectId;
	};

	// #############################################################################
	// renderbuffer objectstuct:
	// #############################################################################

	struct RenderbufferObject : Resource
	{
	private:
		friend struct ResourceManager;
		friend struct Framebuffer;
		// Framebuffer(const unsigned int& id);
		// static std::shared_ptr<Framebuffer> Initialise() noexcept;
		// const unsigned int AttachRenderObject(const GLenum& attachmentPoint) noexcept;
		// const unsigned int UtiliseRenderObject(const GLenum& internalFormat, const glm::ivec2& dimensions) noexcept;
		const int Create(const TextureAttachment& attachmentType);
		const int Load(const std::string& path) override;

		unsigned int _id = 0;
		GLenum _attachmentType;
	};
}