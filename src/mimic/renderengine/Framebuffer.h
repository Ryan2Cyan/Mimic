#pragma once
#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <memory>

namespace Mimic
{
	// #############################################################################
	// framebuffer stuct:
	// #############################################################################
	struct Framebuffer
	{
		Framebuffer(const unsigned int& id);
		static std::shared_ptr<Framebuffer> Initialise() noexcept;
		const unsigned int AttachRenderObject(const GLenum& attachmentPoint) noexcept;
		const unsigned int UtiliseRenderObject(const GLenum& internalFormat, const glm::ivec2& dimensions) noexcept;
	private:
		const unsigned int _id;
		unsigned int _renderObjectId;
	};
}