#pragma once
#include <memory>
#include <glm/glm.hpp>
#include <vector>

namespace MimicRender
{
	struct RenderTexture;
	struct Shader;
	struct Model;
	struct DirectLight;
	struct Renderer;

	/// <summary>
	/// Generates depth maps based on the light sources within the scene, to render cast shadows.
	/// </summary>
	struct ShadowMapper
	{
		static const std::shared_ptr<ShadowMapper> Initialise(glm::ivec2 depthMapDimensions = glm::ivec2(2048, 2048));

		/// <summary>  
		/// Render all scene objects from the perspective (light space) of each direct light. Only the depth component
		/// is rendered. The result depth maps are stored within the DirectLight struct. 
		/// </summary>
		void RenderDirectLightDepthMaps(const std::vector<std::shared_ptr<Model>>& models, const std::vector<std::shared_ptr<DirectLight>>& directLights, std::shared_ptr<Renderer>& renderer, const glm::ivec2& clippingPlanes = glm::ivec2(1.0f, 100.0f));

		/// <summary>  
		/// Render all scene objects from the perspective (light space) of each direct light. Only the depth component
		/// is rendered. The result depth maps are stored within the DirectLight struct. 
		/// </summary>
		void RenderDirectLightDepthMaps(const std::vector<std::shared_ptr<DirectLight>>& directLights, std::shared_ptr<Renderer>& renderer, const glm::ivec2& clippingPlanes = glm::ivec2(1.0f, 100.0f));

	private:
		std::shared_ptr<Shader> _depthMapShader;
		glm::ivec2 _depthMapDimensions;
		bool _initialised;
	};
}