#include "ShadowMapper.h"
#include <renderengine/RenderTexture.h>
#include <renderengine/Texture.h>
#include <renderengine/Shader.h>
#include <renderengine/Model.h>
#include <renderengine/Renderer.h>
#include <utility/Logger.h>
#include <utility/FileLoader.h>
#include <filesystem>
#include <string>


namespace MimicRender
{
	const std::shared_ptr<ShadowMapper> ShadowMapper::Initialise(glm::ivec2 aspectRatio)
	{
		std::shared_ptr<RenderTexture> depthMapRenderTexture = RenderTexture::Initialise();
		depthMapRenderTexture->SetTexture(Texture::Initialise(aspectRatio, Texture::MIMIC_DEPTH_MAP_PARAMS, Texture::MIMIC_DEPTH_COMPONENT, Texture::MIMIC_DEPTH_COMPONENT));
		depthMapRenderTexture->AttachTexture(TextureTarget::MIMIC_TEXTURE_2D, RenderTexture::MIMIC_NO_DRAW | RenderTexture::MIMIC_NO_READ | RenderTexture::MIMIC_DEPTH_BUFFER_BIT, RenderTextureAttachment::MIMIC_DEPTH);
		depthMapRenderTexture->Unbind();

		if (depthMapRenderTexture == nullptr)
		{
			MIMIC_LOG_WARNING("[MimicRender::Texture] Unable to initialise depth map render texture.");
			return nullptr;
		}

		std::shared_ptr<Mimic::FileLoader> fileLoader = Mimic::FileLoader::Initialise();
		const std::string assetPath = fileLoader->LocateDirectory("assets").generic_string();

		std::shared_ptr<ShadowMapper> shadowMapper = std::make_shared<ShadowMapper>();
		shadowMapper->_depthMapRenderTexture = depthMapRenderTexture;
		shadowMapper->_depthMapShader = Shader::Initialise(fileLoader->LocateFileInDirectory(assetPath, "DepthMapShader.glsl"));
		shadowMapper->_initialised = true;
		return shadowMapper;
	}

	const void GetDepthMapTextureId(const std::vector<std::shared_ptr<Model>>& models, const std::vector<std::shared_ptr<DirectLight>>& directLights, std::shared_ptr<Renderer>& renderer)
	{
		for (auto light : directLights)
		{
			for (auto model : models)
			{

			}
		}
	}

	const unsigned int ShadowMapper::GetDepthMapTextureId() const noexcept
	{
		if (!_initialised) return 0;
		return _depthMapRenderTexture->GetTextureID();
	}
}