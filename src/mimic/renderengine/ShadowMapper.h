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

	struct ShadowMapper
	{
		static const std::shared_ptr<ShadowMapper> Initialise(glm::ivec2 aspectRatio = glm::ivec2(1024, 1024));
		// void CaptureDepthMap()
		const void  GetDepthMapTextureId(const std::vector<std::shared_ptr<Model>>& models, const std::vector<std::shared_ptr<DirectLight>>& directLights, std::shared_ptr<Renderer>& renderer);
		const unsigned int GetDepthMapTextureId() const noexcept;

	private:
		std::shared_ptr<RenderTexture> _depthMapRenderTexture;
		std::shared_ptr<Shader> _depthMapShader;
		bool _initialised;
		
	};
}