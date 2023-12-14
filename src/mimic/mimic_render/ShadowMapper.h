#pragma once
#include <memory>
#include <glm/glm.hpp>
#include <vector>

// #############################################################################
// shadow mapper stuct:
// #############################################################################

namespace MimicRender
{
	struct RenderTexture;
	struct Shader;
	struct Model;
	struct DirectLight;
	struct Renderer;

	struct ShadowMapper
	{
		static const std::shared_ptr<ShadowMapper> Initialise(glm::ivec2 aspectRatio = glm::ivec2(1024, 1024));

		/// <summary>
		/// Render all scene objects from the perspective (light space) of each direct light. Only the depth component
		/// is rendered. The result depth maps are stored within the DirectLight struct. 
		/// </summary>
		void RenderDirectLightDepthMaps(const std::vector<std::shared_ptr<Model>>& models, std::vector<std::shared_ptr<DirectLight>>& directLights, std::shared_ptr<Renderer>& renderer, const glm::ivec2& clippingPlanes = glm::ivec2(1.0f, 100.0f));

	private:
		std::shared_ptr<Shader> _depthMapShader;
		glm::ivec2 _depthMapAspect;
		bool _initialised;
	};
}