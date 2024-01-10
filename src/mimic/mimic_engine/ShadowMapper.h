#pragma once
#include <memory>
#include <vector>
#include <glm/glm.hpp>

namespace MimicRender
{
	struct ShadowMapper;
	struct DirectLight;
	struct Renderer;
}

namespace MimicEngine
{
	/// <summary>
	/// Interface for MimicRender::ShadowMapper. Generates depth maps based on the light sources within the scene, to 
	/// render cast shadows.
	/// </summary>
	struct ShadowMapper
	{
	private:
		friend struct MimicCore;

		static std::shared_ptr<ShadowMapper> Initialise(glm::ivec2 depthMapDimensions = glm::ivec2(2048, 2048));
		void RenderDirectLightDepthMaps(std::shared_ptr<MimicRender::Renderer>& renderer, const glm::ivec2& clippingPlanes = glm::ivec2(1.0f, 100.0f));

		std::vector<std::shared_ptr<MimicRender::DirectLight>> _depthMapDirectLights;
		std::shared_ptr<MimicRender::ShadowMapper> _renderShadowMapper;
	};
}