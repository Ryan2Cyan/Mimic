#include "CubeMap.h"
#include <lowlevelsystems/MimicCore.h>
#include <lowlevelsystems/ResourceManager.h>
#include <utility/FileLoader.h>
#include <renderengine/Shader.h>
#include <utility/Logger.h>
#include <stb_image.h>
#include <GL/glew.h>
#include <memory>
#include <filesystem>

namespace Mimic
{
	CubeMap::CubeMap() : _initialised(false), _skipDraw(false)
	{
		_faceTextures = {"", "", "", "", "", ""};
		_shader = MimicCore::ResourceManager->LoadResource<Shader>("CubeMapShader.glsl");
		glUseProgram(_shader->_shaderProgramId);
		_shader->SetInt("skybox", 0);
		glUseProgram(0);
	}

	void CubeMap::SetFaceTexture(const CubeMapFace& face, const std::string& textureFileName)
	{
		_faceTextures[(int)face] = textureFileName;
	}

	void CubeMap::Load()
	{
		constexpr float skyboxVertices[] =
		{
			//   Coordinates
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f
		};

		constexpr unsigned int skyboxIndices[] =
		{
			// Right
			1, 2, 6,
			6, 5, 1,
			// Left
			0, 4, 7,
			7, 3, 0,
			// Top
			4, 5, 6,
			6, 7, 4,
			// Bottom
			0, 3, 2,
			2, 1, 0,
			// Back
			0, 1, 5,
			5, 4, 0,
			// Front
			3, 7, 6,
			6, 2, 3
		};
		
		unsigned int skyboxVBO;
		unsigned int skyboxEBO;
		glGenVertexArrays(1, &_skyboxVertexArrayId);
		glGenBuffers(1, &skyboxVBO);
		glGenBuffers(1, &skyboxEBO);
		glBindVertexArray(_skyboxVertexArrayId);
		glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glGenTextures(1, &_cubeMapTextureId);
		glBindTexture(GL_TEXTURE_CUBE_MAP, _cubeMapTextureId);
		FileLoader fileLoader;
		for (unsigned int i = 0; i < _faceTextures.size(); i++)
		{
			const std::string filePath = fileLoader.LocateFileInDirectory(MimicCore::ResourceManager->_assetsDirectory, _faceTextures[i]);
			int width, height, nrChannels;
			unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
			if (data)
			{
				//stbi_set_flip_vertically_on_load(false);
				glTexImage2D(
					GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0,
					GL_RGB,
					width,
					height,
					0,
					GL_RGB,
					GL_UNSIGNED_BYTE,
					data
				);
				stbi_image_free(data);
			}
			else
			{
				MIMIC_LOG_WARNING("[Mimic::CubeMap] Cubemap texture failed to load at path: \"%\"", filePath);
				stbi_image_free(data);
			}
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		_initialised = true;
	}

	void CubeMap::Draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
	{
		if (_skipDraw) return;
		if (!_initialised)
		{
			MIMIC_LOG_WARNING("[Mimic::CubeMap] Is uninitialised, therefore unable to draw.");
			_skipDraw = true;
		}

		glDepthFunc(GL_LEQUAL);
		glUseProgram(_shader->_shaderProgramId);
		glm::mat4 view = glm::mat4(glm::mat3(viewMatrix));
		_shader->SetMat4("u_View", view);
		_shader->SetMat4("u_Projection", projectionMatrix);

		glBindVertexArray(_skyboxVertexArrayId);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, _cubeMapTextureId);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glDepthFunc(GL_LESS);
	}
}