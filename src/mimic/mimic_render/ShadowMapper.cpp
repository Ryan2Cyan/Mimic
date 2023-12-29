#include "ShadowMapper.h"
#include <mimic_render/RenderTexture.h>
#include <mimic_render/Shader.h>
#include <mimic_render/Model.h>
#include <mimic_render/Renderer.h>
#include <mimic_render/Light.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <mimic_utility/Logger.h>
#include <mimic_utility/FileLoader.h>


namespace MimicRender
{
	const std::shared_ptr<ShadowMapper> ShadowMapper::Initialise(glm::ivec2 depthMapDimensions)
	{
		// Initialise file loader struct and use it to store a path to the project's "assets/" directory:
		std::shared_ptr<MimicUtility::FileLoader> fileLoader = MimicUtility::FileLoader::Initialise();
		const std::string assetPath = fileLoader->LocateDirectory("assets").generic_string();


		std::shared_ptr<ShadowMapper> shadowMapper = std::make_shared<ShadowMapper>();

		// Load depth map shader:
		shadowMapper->_depthMapShader = Shader::Initialise(fileLoader->LocateFileInDirectory(assetPath, "DepthMapShader.glsl"));

		shadowMapper->_depthMapDimensions = depthMapDimensions;
		shadowMapper->_initialised = true;
		return shadowMapper;
	}

	void ShadowMapper::RenderDirectLightDepthMaps(const std::vector<std::shared_ptr<Model>>& models, const std::vector<std::shared_ptr<DirectLight>>& directLights, std::shared_ptr<Renderer>& renderer, const glm::ivec2& clippingPlanes)
	{
		// Instantiate base light matrix parameters (lightView & lightProjection). These will
		// be needed to render the scene from the light's POV:
		const glm::vec2 lightProjectionClippingPlanes = clippingPlanes;
		const glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, lightProjectionClippingPlanes.x, lightProjectionClippingPlanes.y); // Note: Potentially add parameters to edit the parameters of the orthographic projection.
		glm::mat4 lightView = glm::mat4(1.0f);

		// The renderer requires an on draw lambda to render the scene. Here a simple one is
		// created that just sets the vertex shader parameter of the light matrix:
		std::function<void()> depthMapOnDrawLamba = [&]()
		{
			_depthMapShader->SetMat4("u_LightSpaceMatrix", lightProjection * lightView);
		};
		
		// Loop over every light to assign it's unique depth map:
		for (auto directLight : directLights)
		{
			// Assign view matrix. The projection matrix does not change as it is orthographic:
			lightView = glm::lookAt(directLight->Position, directLight->Position + directLight->Direction, glm::vec3(0.0f, 1.0f, 0.0f));
			directLight->_lightMatrix = lightProjection * lightView;

			// Initialise light's render texture to store the depth map:
			if (directLight->_depthMapRT == nullptr)
			{
				directLight->_depthMapRT = RenderTexture::Initialise();
				directLight->_depthMapRT->SetTexture(Texture::Initialise(_depthMapDimensions, Texture::MIMIC_DEPTH_MAP_PARAMS, Texture::MIMIC_DEPTH_COMPONENT, Texture::MIMIC_DEPTH_COMPONENT));
			}

			// Bind light's render texture in preparation for rendering:
			directLight->_depthMapRT->AttachTexture(TextureTarget::MIMIC_TEXTURE_2D, RenderTexture::MIMIC_NO_DRAW | RenderTexture::MIMIC_NO_READ | RenderTexture::MIMIC_DEPTH_BUFFER_BIT, RenderTextureAttachment::MIMIC_DEPTH);
			directLight->_depthMapRT->SetTextureViewPort();

			// Queue all models to be rendered into each direct light's 2D depth map:
			for (auto model : models) model->QueueMeshesToDraw(_depthMapShader, depthMapOnDrawLamba, renderer);
			
			glClear(GL_DEPTH_BUFFER_BIT);

			// Cull front-faces to reduce the effects of Peter-Panning. Doesn't matter is shadows are rendered
			// inside objects.
			glCullFace(GL_FRONT);

			// Render the scene into the depth map:
			renderer->Draw(lightView, lightProjection);
			renderer->ClearRenderQueue();
			glCullFace(GL_BACK);

			directLight->_depthMapRT->Unbind();
		}
	}

	void ShadowMapper::RenderDirectLightDepthMaps(const std::vector<std::shared_ptr<DirectLight>>& directLights, std::shared_ptr<Renderer>& renderer, const glm::ivec2& clippingPlanes)
	{
		// Instantiate base light matrix parameters (lightView & lightProjection). These will
		// be needed to render the scene from the light's POV:
		const glm::vec2 lightProjectionClippingPlanes = clippingPlanes;
		const glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, lightProjectionClippingPlanes.x, lightProjectionClippingPlanes.y); // Note: Potentially add parameters to edit the parameters of the orthographic projection.
		glm::mat4 lightView = glm::mat4(1.0f);

		// The renderer requires an on draw lambda to render the scene. Here a simple one is
		// created that just sets the vertex shader parameter of the light matrix:
		std::function<void()> depthMapOnDrawLamba = [&]()
		{
			_depthMapShader->SetMat4("u_LightSpaceMatrix", lightProjection * lightView);
		};

		// Loop over every light to assign it's unique depth map:
		for (auto directLight : directLights)
		{
			// Assign view matrix. The projection matrix does not change as it is orthographic:
			lightView = glm::lookAt(directLight->Position, directLight->Position + directLight->Direction, glm::vec3(0.0f, 1.0f, 0.0f));
			directLight->_lightMatrix = lightProjection * lightView;

			// Initialise light's render texture to store the depth map:
			if (directLight->_depthMapRT == nullptr)
			{
				directLight->_depthMapRT = RenderTexture::Initialise();
				directLight->_depthMapRT->SetTexture(Texture::Initialise(_depthMapDimensions, Texture::MIMIC_DEPTH_MAP_PARAMS, Texture::MIMIC_DEPTH_COMPONENT, Texture::MIMIC_DEPTH_COMPONENT));
			}

			// Bind light's render texture in preparation for rendering:
			directLight->_depthMapRT->AttachTexture(TextureTarget::MIMIC_TEXTURE_2D, RenderTexture::MIMIC_NO_DRAW | RenderTexture::MIMIC_NO_READ | RenderTexture::MIMIC_DEPTH_BUFFER_BIT, RenderTextureAttachment::MIMIC_DEPTH);
			directLight->_depthMapRT->SetTextureViewPort();

			glClear(GL_DEPTH_BUFFER_BIT);

			// Cull front-faces to reduce the effects of Peter-Panning. Doesn't matter is shadows are rendered
			// inside objects.
			glCullFace(GL_FRONT);

			// Render the scene into the depth map:
			renderer->Draw(lightView, lightProjection);
			glCullFace(GL_BACK);

			directLight->_depthMapRT->Unbind();
		}
	}
}