#include "ShadowMapper.h"
#include "MimicCore.h"

namespace MimicEngine
{
	std::shared_ptr<ShadowMapper> ShadowMapper::Initialise(glm::ivec2 depthMapDimensions)
	{
		std::shared_ptr<ShadowMapper> shadowMapper = std::make_shared<ShadowMapper>();
		shadowMapper->_renderShadowMapper = MimicRender::ShadowMapper::Initialise(depthMapDimensions);
		return shadowMapper;
	}

	void ShadowMapper::RenderDirectLightDepthMaps(const glm::ivec2& clippingPlanes)
	{
		_renderShadowMapper->RenderDirectLightDepthMaps(_depthMapDirectLights, MimicCore::_renderer, clippingPlanes);
	}
}