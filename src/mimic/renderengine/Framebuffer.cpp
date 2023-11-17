#include "Framebuffer.h"
#include <utility/Logger.h>

namespace Mimic
{
	Framebuffer::Framebuffer(const unsigned int& id) : _id(id), _renderObjectId(0) { }

	std::shared_ptr<Framebuffer> Framebuffer::Initialise() noexcept
	{
		unsigned int framebufferId;
		glGenFramebuffers(1, &framebufferId);
		return std::make_shared<Framebuffer>(framebufferId);
	}

	const unsigned int Framebuffer::AttachRenderObject(const GLenum& attachmentPoint) noexcept
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _id);
		glGenRenderbuffers(1, &_renderObjectId);
		glBindRenderbuffer(GL_RENDERBUFFER, _renderObjectId);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachmentPoint, GL_RENDERBUFFER, _renderObjectId);
		return _renderObjectId;
	}

	const unsigned int Framebuffer::UtiliseRenderObject(const GLenum& internalFormat, const glm::ivec2& dimensions) noexcept
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _id);
		if (_renderObjectId == 0)
		{
			MIMIC_LOG_WARNING("[Mimic::Framebuffer] Unable to allocate storage to uninitialised render buffer object.");
			return 0;
		}
		glBindRenderbuffer(GL_RENDERBUFFER, _renderObjectId);
		glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, dimensions.x, dimensions.y);
		glViewport(0, 0, dimensions.x, dimensions.y);
		MIMIC_LOG_OPENGL("Framebuffer");
		return _renderObjectId;
	}
}