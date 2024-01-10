#pragma once
#include <GLM/glm.hpp>
#include <array>
#include <functional>

namespace MimicRender
{
	// Source: https://learnopengl.com/Advanced-OpenGL/Cubemaps
	// Source: https://learnopengl.com/PBR/IBL/Diffuse-irradiance

	struct RenderTexture;
	struct Renderer;
	struct Shader;

	/// <summary>
	/// Renders six faces of a cube map (appearing as a background). A file name of a .hdr file can be passed in 
	/// to this struct which will generate the cube map.
	/// </summary>
	struct EnvironmentCubeMap
	{
		static std::shared_ptr<EnvironmentCubeMap> Initialise(const std::string& hdrFileName, const glm::vec2& aspectRatio, std::shared_ptr<Renderer>& renderer);

		/// <summary>
		/// Get the ID for the diffuse irradiance map.
		/// </summary>
		unsigned int GetIrradianceId() const;

		/// <summary>
		/// Get the ID for the specular prefiltered map.
		/// </summary>
		unsigned int GetPreFilteredId() const;

		/// <summary>
		/// Get the ID for the specular 2D BRDF look-up texture.
		/// </summary>
		unsigned int GetBRDFId() const;

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