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

		bool LoadUnitCube();
		bool LoadCubeMapTexture();
		bool LoadShader(const std::string& fileName);
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

	struct HDRCubeMap : CubeMap
	{
		void Load(const std::string& equirectangularTextureFileName);
		void Draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);
		
	private:
		bool LoadEquirectangular(const std::string& fileName);

		std::shared_ptr<Texture> _equirectangularTexture;
	};
}