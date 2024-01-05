#pragma once
#include <mimic_render/ShadowMapper.h>

namespace MimicEngine
{
	struct Model;
	struct DirectLight;
	struct MimicRender::Model;
	struct MimicRender::DirectLight;
	struct MimicRender::Renderer;

	// #############################################################################
	// Shadow Mapper Struct:
	// #############################################################################
	struct ShadowMapper
	{
		static std::shared_ptr<ShadowMapper> Initialise(glm::ivec2 depthMapDimensions = glm::ivec2(2048, 2048));

	private:
		friend struct MimicCore;
		void RenderDirectLightDepthMaps(std::shared_ptr<MimicRender::Renderer>& renderer, const glm::ivec2& clippingPlanes = glm::ivec2(1.0f, 100.0f));

		std::vector<std::shared_ptr<MimicRender::DirectLight>> _depthMapDirectLights;
		std::shared_ptr<MimicRender::ShadowMapper> _renderShadowMapper;
	};
}