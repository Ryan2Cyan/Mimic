#include "Framebuffer.h"
#include <utility/Logger.h>

namespace Mimic
{
	// #############################################################################
	// framebuffer functions:
	// #############################################################################
	const int Framebuffer::Create()
	{
		glGenFramebuffers(1, &_id);
		return _id != 0;
	}

	const int Framebuffer::Load(const std::string& path)
	{
		return -1;
	}

	void Framebuffer::Unbind() const noexcept
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Framebuffer::AttachRenderObject(const std::shared_ptr<RenderbufferObject>& renderbufferObject)
	{
		if (renderbufferObject->_id == 0)
		{
			MIMIC_LOG_WARNING("[Mimic::Framebuffer] Could not attach renderbuffer object, ID invalid");
			return;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, _id);
		glBindRenderbuffer(GL_RENDERBUFFER, renderbufferObject->_id);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, renderbufferObject->_attachmentType, GL_RENDERBUFFER, renderbufferObject->_id);
		_boundRenderbufferObjectId = _id;

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	void Framebuffer::RenderToTexture(const unsigned int& textureId, const TextureAttachment& textureAttachment, const TextureTarget& textureTarget, const int level)
	{

		GLenum attachmentTypeGL = 0;
		switch (textureAttachment)
		{
			case TextureAttachment::MIMIC_DEPTH:{ attachmentTypeGL = GL_DEPTH_ATTACHMENT; } break;
			case TextureAttachment::MIMIC_DEPTH_STENCIL: { attachmentTypeGL = GL_DEPTH_STENCIL_ATTACHMENT; } break;
			case TextureAttachment::MIMIC_STENCIL: { attachmentTypeGL = GL_STENCIL_ATTACHMENT; } break;
			case TextureAttachment::MIMIC_COLOR0: { attachmentTypeGL = GL_COLOR_ATTACHMENT0; } break;
			default:
			{
				MIMIC_LOG_WARNING("[Mimic::RenderbufferObject] Could not render to texture, invalid attachment type.");
				return;
			}break;
		}

		GLenum targerGL = 0;
		switch (textureTarget)
		{
			case TextureTarget::MIMIC_TEXTURE_2D: { targerGL = GL_TEXTURE_2D; } break;
			case TextureTarget::MIMIC_CUBE_MAP_POSITIVE_X: { targerGL = GL_TEXTURE_CUBE_MAP_POSITIVE_X; } break;
			case TextureTarget::MIMIC_CUBE_MAP_NEGATIVE_X: { targerGL = GL_TEXTURE_CUBE_MAP_NEGATIVE_X; } break;
			case TextureTarget::MIMIC_CUBE_MAP_POSITIVE_Y: { targerGL = GL_TEXTURE_CUBE_MAP_POSITIVE_Y; } break;
			case TextureTarget::MIMIC_CUBE_MAP_NEGATIVE_Y: { targerGL = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y; } break;
			case TextureTarget::MIMIC_CUBE_MAP_POSITIVE_Z: { targerGL = GL_TEXTURE_CUBE_MAP_POSITIVE_Z; } break;
			case TextureTarget::MIMIC_CUBE_MAP_NEGATIVE_Z: { targerGL = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z; } break;
			default:
			{
				MIMIC_LOG_WARNING("[Mimic::RenderbufferObject] Could not render to texture, invalid target type.");
				return;
			}break;
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentTypeGL, targerGL, textureId, level);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Framebuffer::UseRenderObject(const glm::ivec2& aspectRatio) const
	{
		if (_boundRenderbufferObjectId == 0)
		{
			MIMIC_LOG_WARNING("[Mimic::Framebuffer] Could not use bound renderbuffer object, ID invalid");
			return;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, _id);
		glBindRenderbuffer(GL_RENDERBUFFER, _boundRenderbufferObjectId);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, aspectRatio.x, aspectRatio.y);
		glViewport(0, 0, aspectRatio.x, aspectRatio.y);
	}

	// #############################################################################
	// renderbuffer object functions:
	// #############################################################################
	const int RenderbufferObject::Create(const TextureAttachment& attachmentType)
	{
		glGenRenderbuffers(1, &_id);
		switch (attachmentType)
		{
			case TextureAttachment::MIMIC_DEPTH:
			{ 
				_attachmentType = GL_DEPTH_ATTACHMENT;
			}break;
			case TextureAttachment::MIMIC_DEPTH_STENCIL:
			{
				_attachmentType = GL_DEPTH_STENCIL_ATTACHMENT;
			}break;
			case TextureAttachment::MIMIC_STENCIL:
			{
				_attachmentType = GL_STENCIL_ATTACHMENT;
			}break;
			default:
			{
				MIMIC_LOG_WARNING("[Mimic::RenderbufferObject] Could not create renderbuffer object, invalid attachment type.");
				return -1;
			}break;
		}
		return _id != 0;
	}

	const int RenderbufferObject::Load(const std::string& path)
	{
		return -1;
	}
	/*std::shared_ptr<Framebuffer> Framebuffer::Initialise() noexcept
	{
		unsigned int framebufferId;
		glGenFramebuffers(1, &framebufferId);
		return std::make_shared<Framebuffer>(framebufferId);
	}*/

	//const unsigned int Framebuffer::AttachRenderObject(const GLenum& attachmentPoint) noexcept
	//{
	//	glBindFramebuffer(GL_FRAMEBUFFER, _id);
	//	glGenRenderbuffers(1, &_renderObjectId);
	//	glBindRenderbuffer(GL_RENDERBUFFER, _renderObjectId);
	//	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachmentPoint, GL_RENDERBUFFER, _renderObjectId);
	//	return _renderObjectId;
	//}

	//const unsigned int Framebuffer::UtiliseRenderObject(const GLenum& internalFormat, const glm::ivec2& dimensions) noexcept
	//{
	//	glBindFramebuffer(GL_FRAMEBUFFER, _id);
	//	if (_renderObjectId == 0)
	//	{
	//		MIMIC_LOG_WARNING("[Mimic::Framebuffer] Unable to allocate storage to uninitialised render buffer object.");
	//		return 0;
	//	}
	//	glBindRenderbuffer(GL_RENDERBUFFER, _renderObjectId);
	//	glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, dimensions.x, dimensions.y);
	//	glViewport(0, 0, dimensions.x, dimensions.y);
	//	MIMIC_LOG_OPENGL("Framebuffer");
	//	return _renderObjectId;
	//}
}