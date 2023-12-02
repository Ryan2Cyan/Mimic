#pragma once
#include <memory>
#include <functional>
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

	struct DirectLightDepthMapData
	{
		explicit DirectLightDepthMapData(const std::shared_ptr<RenderTexture>& depthMap, const glm::mat4& lightMatrix);
		std::shared_ptr<RenderTexture> _depthMapRT;
		glm::mat4 _lightMatrix;
	};

	struct ShadowMapper
	{
		static const std::shared_ptr<ShadowMapper> Initialise(glm::ivec2 aspectRatio = glm::ivec2(1024, 1024));
		void RenderDirectLightDepthMaps(const std::vector<std::shared_ptr<Model>>& models, std::vector<std::shared_ptr<DirectLight>>& directLights, std::shared_ptr<Renderer>& renderer);

	private:
		std::shared_ptr<Shader> _depthMapShader;
		glm::ivec2 _depthMapAspect;
		bool _initialised;
	};
}