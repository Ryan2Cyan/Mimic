#include "ShadowMapper.h"
#include <renderengine/RenderTexture.h>
#include <renderengine/Texture.h>
#include <renderengine/Shader.h>
#include <renderengine/Model.h>
#include <renderengine/Renderer.h>
#include <renderengine/Light.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <utility/Logger.h>
#include <utility/FileLoader.h>
#include <filesystem>
#include <string>


namespace MimicRender
{
	DirectLightDepthMapData::DirectLightDepthMapData(const std::shared_ptr<RenderTexture>& depthMap, const glm::mat4& lightMatrix)
		: _depthMapRT(depthMap), _lightMatrix(lightMatrix)
	{

	}

	const std::shared_ptr<ShadowMapper> ShadowMapper::Initialise(glm::ivec2 aspectRatio)
	{
		std::shared_ptr<MimicUtil::FileLoader> fileLoader = MimicUtil::FileLoader::Initialise();
		const std::string assetPath = fileLoader->LocateDirectory("assets").generic_string();

		std::shared_ptr<ShadowMapper> shadowMapper = std::make_shared<ShadowMapper>();
		shadowMapper->_depthMapShader = Shader::Initialise(fileLoader->LocateFileInDirectory(assetPath, "DepthMapShader.glsl"));
		shadowMapper->_initialised = true;
		shadowMapper->_depthMapAspect = aspectRatio;
		return shadowMapper;
	}

	void ShadowMapper::RenderDirectLightDepthMaps(const std::vector<std::shared_ptr<Model>>& models, std::vector<std::shared_ptr<DirectLight>>& directLights, std::shared_ptr<Renderer>& renderer)
	{
		constexpr glm::vec2 lightProjectionClippingPlanes = glm::vec2(1.0f, 25.0f);
		const glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, lightProjectionClippingPlanes.x, lightProjectionClippingPlanes.y);
		glm::mat4 lightView = glm::mat4(1.0f);

		std::function<void()> depthMapOnDrawLamba = [&]()
		{
			_depthMapShader->SetMat4("u_LightSpaceMatrix", lightProjection * lightView);
		};
		
		// que all models to be rendered into each direct light's depth map textures:
		for (auto directLight : directLights)
		{
			lightView = glm::lookAt(directLight->Position, directLight->Position + directLight->Direction, glm::vec3(0.0f, 1.0f, 0.0f));
			directLight->_lightMatrix = lightProjection * lightView;

			if (directLight->_depthMapRT == nullptr)
			{
				directLight->_depthMapRT = RenderTexture::Initialise();
				directLight->_depthMapRT->SetTexture(Texture::Initialise(_depthMapAspect, Texture::MIMIC_DEPTH_MAP_PARAMS, Texture::MIMIC_DEPTH_COMPONENT, Texture::MIMIC_DEPTH_COMPONENT));
			}
			directLight->_depthMapRT->AttachTexture(TextureTarget::MIMIC_TEXTURE_2D, RenderTexture::MIMIC_NO_DRAW | RenderTexture::MIMIC_NO_READ | RenderTexture::MIMIC_DEPTH_BUFFER_BIT, RenderTextureAttachment::MIMIC_DEPTH);
			directLight->_depthMapRT->SetTextureViewPort();

			for (auto model : models) model->QueMeshesToDraw(_depthMapShader, depthMapOnDrawLamba, renderer);
			
			glClear(GL_DEPTH_BUFFER_BIT);
			glCullFace(GL_FRONT); // cull front-faces to avoid Peter-Panning:
			renderer->Draw(lightView, lightProjection);
			renderer->ClearRenderQue();
			glCullFace(GL_BACK);

			directLight->_depthMapRT->Unbind();
		}
	}
}