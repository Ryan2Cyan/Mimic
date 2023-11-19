#pragma once
#include <GLM/glm.hpp>
#include <array>
#include <string>
#include <memory>

namespace Mimic
{
	// #############################################################################
	// cubemap stuct:
	// #############################################################################
	// Source: https://learnopengl.com/Advanced-OpenGL/Cubemaps
	enum class CubeMapFace
	{
		FaceRight = 0, 
		FaceLeft = 1, 
		FaceTop = 2, 
		FaceBottom = 3, 
		FaceFront = 4, 
		FaceBack = 5
	};

	struct Shader;

	struct CubeMap
	{
		explicit CubeMap();
		void SetFaceTexture(const CubeMapFace& face, const std::string& textureFileName);
		void Draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);

	private:
		void Load();

	protected:
		friend struct MimicCore;

		const bool LoadUnitCube();
		const bool LoadCubeMapTexture();
		const bool LoadShader(const std::string& fileName);
		std::array<std::string, 6> _faceTextures;
		std::shared_ptr<Shader> _shader;
		unsigned int _cubeMapTextureId;
		unsigned int _cubeMapVertexArrayId;
		bool _skipDraw;
		bool _initialised;
	};

	// #############################################################################
	// HDR cubemap stuct:
	// #############################################################################
	// Source: https://learnopengl.com/PBR/IBL/Diffuse-irradiance

	struct Texture;
	struct Framebuffer;

	struct EnvironmentCubeMap
	{
		explicit EnvironmentCubeMap();
		static std::shared_ptr<EnvironmentCubeMap> Initialise();
		void Load(const std::string& equirectangularTextureFileName);
		void Draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);
		
	private:
		friend struct PBRMaterial;

		const bool LoadEquirectangular(const std::string& fileName);
		const bool LoadUnitCube();
		void LoadEnvironmentMap();
		void LoadIrradianceMapTexture();
		void LoadPrefilteredMapTexture();
		void LoadBRDFConvolutedTexture();
		const bool LoadShader(const std::string& fileName, std::shared_ptr<Shader>& shader);

		void RenderUnitCube() const noexcept;
		void RenderQuad();

		std::array<glm::mat4, 6> _captureViews;
		glm::mat4 _captureProjection;
		std::shared_ptr<Texture> _equirectangularTexture;
		std::shared_ptr<Texture> _environmentMapTexture;
		std::shared_ptr<Texture> _irradianceMapTexture;
		std::shared_ptr<Texture> _prefilteredMapTexture;
		std::shared_ptr<Texture> _brdfConvolutedTexture;

		std::shared_ptr<Shader> _brdfConvolutionShader;
		std::shared_ptr<Shader> _preFilteredShader;
		std::shared_ptr<Shader> _equirectangularToCubemapShader;
		std::shared_ptr<Shader> _convolutionShader;
		std::shared_ptr<Shader> _cubeMapShader;

		std::shared_ptr<Framebuffer> _framebuffer;

		unsigned int _unitQuadVertexArrayId;
		unsigned int _unitCubeVertexArrayId;
		unsigned int _framebufferId;
		unsigned int _renderObjectId;

		bool _skipDraw;
		bool _initialised;
	};
}