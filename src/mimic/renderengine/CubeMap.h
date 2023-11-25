#pragma once
#include <GLM/glm.hpp>
#include <array>
#include <string>
#include <memory>

namespace MimicRender
{
	// #############################################################################
	// HDR cubemap stuct:
	// #############################################################################
	// Source: https://learnopengl.com/Advanced-OpenGL/Cubemaps
	// Source: https://learnopengl.com/PBR/IBL/Diffuse-irradiance

	struct Texture;
	struct RenderTexture;
	struct RenderbufferObject;
	struct Renderer;

	struct EnvironmentCubeMap
	{
		static const std::shared_ptr<EnvironmentCubeMap> Initialise(const std::string& hdrFileName, const glm::vec2& aspectRatio, std::shared_ptr<Renderer>& renderer);
		void Draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, std::shared_ptr<Renderer>& renderer);
		const unsigned int GetIrradianceId() const;
		const unsigned int GetPreFilteredId() const;
		const unsigned int GetBRDFId() const;
	private:
		friend struct Shader;
		friend struct Renderer;

		void Load(const std::string& equirectangularTextureFileName, const glm::vec2& aspectRatio, std::shared_ptr<Renderer>& renderer);
		void LoadEnvironmentMap(std::shared_ptr<Renderer>& renderer);
		void LoadIrradianceMapTexture(std::shared_ptr<Renderer>& renderer);
		void LoadPrefilteredMapTexture(std::shared_ptr<Renderer>& renderer);
		void LoadBRDFConvolutedTexture(std::shared_ptr<Renderer>& renderer);
		//	const bool LoadShader(const std::string& fileName, std::shared_ptr<Shader>& shader);

		std::array<glm::mat4, 6> _captureViews;
		glm::mat4 _captureProjection;
		glm::vec2 _aspectRatio;
		std::shared_ptr<Texture> _equirectangularTexture;

		std::shared_ptr<Shader> _brdfConvolutionShader;
		std::shared_ptr<Shader> _preFilteredShader;
		std::shared_ptr<Shader> _equirectangularToCubemapShader;
		std::shared_ptr<Shader> _convolutionShader;
		std::shared_ptr<Shader> _cubeMapShader;

		std::shared_ptr<RenderTexture> _environmentMapRenderTexture;
		std::shared_ptr<RenderTexture> _irradianceRenderTexture;
		std::shared_ptr<RenderTexture> _prefilteredMapRenderTexture;
		std::shared_ptr<RenderTexture> _brdfConvolutedRenderTexture;

		bool _skipDraw;
		bool _initialised;
	};
}