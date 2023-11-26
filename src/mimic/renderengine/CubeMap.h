#pragma once
#include <GLM/glm.hpp>
#include <array>
#include <string>
#include <memory>
#include <functional>

namespace MimicRender
{
	// #############################################################################
	// HDR cubemap stuct:
	// #############################################################################
	// Source: https://learnopengl.com/Advanced-OpenGL/Cubemaps
	// Source: https://learnopengl.com/PBR/IBL/Diffuse-irradiance

	struct RenderTexture;
	struct Renderer;
	struct Shader;

	struct EnvironmentCubeMap
	{
		static const std::shared_ptr<EnvironmentCubeMap> Initialise(const std::string& hdrFileName, const glm::vec2& aspectRatio, std::shared_ptr<Renderer>& renderer);
		const unsigned int GetIrradianceId() const;
		const unsigned int GetPreFilteredId() const;
		const unsigned int GetBRDFId() const;
	private:
		friend struct Shader;
		friend struct Renderer;

		static void CaptureCubeMap(std::function<void()>& onDrawLambda, const std::shared_ptr<Shader>& shader, std::shared_ptr<RenderTexture>& renderTexture,
			const glm::ivec2& aspectRatio, std::shared_ptr<Renderer>& renderer, const glm::mat4& captureProjection, const std::array<glm::mat4, 6>& captureViews);
		std::shared_ptr<Shader> _cubeMapShader;

		std::shared_ptr<RenderTexture> _environmentMapRenderTexture;
		std::shared_ptr<RenderTexture> _irradianceRenderTexture;
		std::shared_ptr<RenderTexture> _prefilteredMapRenderTexture;
		std::shared_ptr<RenderTexture> _brdfConvolutedRenderTexture;

		bool _skipDraw;
		bool _initialised;
	};
}