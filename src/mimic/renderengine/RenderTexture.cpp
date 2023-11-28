#include "RenderTexture.h"
#include <utility/Logger.h>

namespace MimicRender
{
	// #############################################################################
	// framebuffer functions:
	// #############################################################################
	std::shared_ptr<RenderTexture> RenderTexture::Initialise() noexcept
	{
		std::shared_ptr<RenderTexture> newFramebuffer = std::make_shared<RenderTexture>();
		glGenFramebuffers(1, &newFramebuffer->_id);
		glGenRenderbuffers(1, &newFramebuffer->_depthRBOId);

		glBindFramebuffer(GL_FRAMEBUFFER, newFramebuffer->_id);
		glBindRenderbuffer(GL_RENDERBUFFER, newFramebuffer->_depthRBOId);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, newFramebuffer->_depthRBOId);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		newFramebuffer->_initialised = true;
		return newFramebuffer;
	}

	void RenderTexture::Bind() const noexcept
	{
		if (!_initialised)
		{
			MIMIC_LOG_WARNING("[MimicRender::RenderTexture] Unable to use uninitialised render texture.");
			return;
		}
		if (_texture == nullptr)
		{
			MIMIC_LOG_WARNING("[MimicRender::RenderTexture] Unable to set uninitialised texture.");
			return;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, _id);
	}

	void RenderTexture::Unbind() const noexcept
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}


	void RenderTexture::BindTextureForRender(const TextureTarget& textureTarget, const std::uint8_t& params, const int level)
	{
		if (_texture == nullptr || !_initialised)
		{
			MIMIC_LOG_WARNING("[MimicRender::RenderTexture] Unable to bind uninitialised texture.");
			return;
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
				MIMIC_LOG_WARNING("[MimicRender::RenderTexture] Could not render to texture, invalid target type.");
				return;
			}break;
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, targerGL, _texture->_id, level);

		if(params & MIMIC_NO_DRAW) glDrawBuffer(GL_NONE);
		if (params & MIMIC_NO_READ) glReadBuffer(GL_NONE);
		if (params & MIMIC_COLOR_BUFFER_BIT) glClear(GL_COLOR_BUFFER_BIT);
		if (params & MIMIC_DEPTH_BUFFER_BIT) glClear(GL_DEPTH_BUFFER_BIT);
	}

	void RenderTexture::UseRenderObject(const glm::ivec2& aspectRatio) const
	{
		if (!_initialised)
		{
			MIMIC_LOG_WARNING("[MimicRender::RenderTexture] Unable to use uninitialised render texture.");
			return;
		}
		if (_depthRBOId == 0)
		{
			MIMIC_LOG_WARNING("[MimicRender::RenderTexture] Could not use bound renderbuffer object, ID invalid");
			return;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, _id);
		glBindRenderbuffer(GL_RENDERBUFFER, _depthRBOId);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, aspectRatio.x, aspectRatio.y);
		glViewport(0, 0, aspectRatio.x, aspectRatio.y);
	}

	void RenderTexture::SetTexture(const std::shared_ptr<Texture>& texture)
	{
		if (!_initialised)
		{
			MIMIC_LOG_WARNING("[MimicRender::RenderTexture] Unable to use uninitialised render texture.");
			return;
		}
		if (texture == nullptr)
		{
			MIMIC_LOG_WARNING("[MimicRender::RenderTexture] Unable to set uninitialised texture.");
			return;
		}
		_texture = texture;
	}

	const unsigned int RenderTexture::GetTextureID() const
	{
		if (!_initialised)
		{
			MIMIC_LOG_WARNING("[MimicRender::RenderTexture] Unable get id of uninitialised texture.");
			return 0;
		}
		return _texture->GetId();
	}
}