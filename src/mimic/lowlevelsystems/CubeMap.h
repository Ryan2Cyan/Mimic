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
		void Load();
		void Draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);


	private:
		friend struct MimicCore;

		std::shared_ptr<Shader> _shader;
		unsigned int _cubeMapTextureId;
		unsigned int _skyboxVertexArrayId;
		std::array<std::string, 6> _faceTextures;
		bool _skipDraw;
		bool _initialised;
	};
}