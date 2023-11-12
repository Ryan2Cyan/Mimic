#include "CubeMap.h"
#include <lowlevelsystems/MimicCore.h>
#include <lowlevelsystems/ResourceManager.h>
#include <utility/FileLoader.h>
#include <renderengine/Shader.h>
#include <renderengine/Model.h>
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
	}

	void CubeMap::SetFaceTexture(const CubeMapFace& face, const std::string& textureFileName)
	{
		_faceTextures[(int)face] = textureFileName;
	}

	void CubeMap::Load()
	{
		_initialised = LoadShader() && LoadUnitCube() && LoadCubeMapTexture();
	}

	bool CubeMap::LoadUnitCube()
	{
		std::shared_ptr<Model> unitCube = MimicCore::ResourceManager->LoadResource<Model>("cube.obj");
		if (unitCube == nullptr)
		{
			MIMIC_LOG_WARNING("[Mimic::Cubemap] Unable to load cubemap unit cube with filename: \"%\"", "cube.obj");
			return false;
		}
		if (unitCube->_meshes.empty())
		{
			MIMIC_LOG_WARNING("[Mimic::Cubemap] Unable to load cubemap unit cube with filename: \"%\", the model contains no loaded meshes.", "cube.obj");
			return false;
		}
		_cubeMapVertexArrayId = unitCube->_meshes[0]->_vertexArrayId;
		return true;
	}

	bool CubeMap::LoadCubeMapTexture()
	{
		glGenTextures(1, &_cubeMapTextureId);
		glBindTexture(GL_TEXTURE_CUBE_MAP, _cubeMapTextureId);
		FileLoader fileLoader;
		for (unsigned int i = 0; i < _faceTextures.size(); i++)
		{
			if (_faceTextures[i] == "")
			{
				MIMIC_LOG_WARNING("[Mimic::Cubemap] Unable to load cubemap texture with filename: \"%\", no file name has been assigned.", _faceTextures[i]);
				return false;
			}
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
				return false;
			}
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		return true;
	}

	bool CubeMap::LoadShader()
	{
		_shader = MimicCore::ResourceManager->LoadResource<Shader>("CubeMapShader.glsl");
		if (_shader == nullptr)
		{
			MIMIC_LOG_WARNING("[Mimic::CubeMap] Unable to load shader with file name: \"%\"", "CubeMapShader.glsl");
			return false;
		}
		glUseProgram(_shader->_shaderProgramId);
		_shader->SetInt("skybox", 0);
		glUseProgram(0);
		return true;
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

		glBindVertexArray(_cubeMapVertexArrayId);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, _cubeMapTextureId);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glDepthFunc(GL_LESS);
	}
}