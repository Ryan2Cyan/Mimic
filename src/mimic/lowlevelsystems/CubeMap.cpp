#include "CubeMap.h"
#include <lowlevelsystems/MimicCore.h>
#include <lowlevelsystems/ResourceManager.h>
#include <lowlevelsystems/Window.h>
#include <utility/FileLoader.h>
#include <renderengine/Shader.h>
#include <renderengine/Model.h>
#include <renderengine/CubemapTexture.h>
#include <renderengine/Framebuffer.h>
#include <utility/Logger.h>
#include <stb_image.h>
#include <GL/glew.h>

namespace Mimic
{
	// #############################################################################
	// cubemap functions:
	// #############################################################################
	CubeMap::CubeMap() : _initialised(false), _skipDraw(false), _cubeMapTextureId(0), _cubeMapVertexArrayId(0)
	{
		_faceTextures = {"", "", "", "", "", ""};
	}

	void CubeMap::SetFaceTexture(const CubeMapFace& face, const std::string& textureFileName)
	{
		_faceTextures[(int)face] = textureFileName;
	}

	void CubeMap::Load()
	{
		_initialised = LoadShader("CubeMapShader.glsl") && LoadUnitCube() && LoadCubeMapTexture();
	}

	const bool CubeMap::LoadUnitCube()
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
		MIMIC_DEBUG_LOG("[OpenGL] Cubemap unit cube loaded with ID: [%]", _cubeMapVertexArrayId);
		return true;
	}

	const bool CubeMap::LoadCubeMapTexture()
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
				MIMIC_LOG_WARNING("[Mimic::CubeMap] Cubemap texture failed to load at path: [%]", filePath);
				stbi_image_free(data);
				return false;
			}
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		MIMIC_DEBUG_LOG("[OpenGL] Cubemap texture generated with ID: \"%\"", _cubeMapTextureId);
		return true;
	}

	const bool CubeMap::LoadShader(const std::string& fileName)
	{
		_shader = MimicCore::ResourceManager->LoadResource<Shader>(fileName);
		if (_shader == nullptr)
		{
			MIMIC_LOG_WARNING("[Mimic::CubeMap] Unable to load shader with file name: \"%\"", "CubeMapShader.glsl");
			return false;
		}
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
		_shader->UseShader();
		_shader->SetInt("u_Skybox", 0);
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




	// #############################################################################
	// hdr cubemap functions:
	// #############################################################################

	EnvironmentCubeMap::EnvironmentCubeMap() : _initialised(false), _skipDraw(false), _unitCubeVertexArrayId(0),
		_framebufferId(0), _renderObjectId(0), _captureProjection(glm::mat4(1.0f)), _unitQuadVertexArrayId(0)
	{
		// set up 6 view matrices (facing each side of the cube), set a projection matrix to 90 fov, and capture each face of the cubemap:
		_captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
		_captureViews =
		{
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
		};
	}

	std::shared_ptr<EnvironmentCubeMap> EnvironmentCubeMap::Initialise() 
	{
		return std::make_shared<EnvironmentCubeMap>();
	}

	void EnvironmentCubeMap::Load(const std::string& equirectangularTextureFileName)
	{
		_initialised = LoadEquirectangular(equirectangularTextureFileName) &&
			LoadShader("EquirectangularToCubemapShader.glsl", _equirectangularToCubemapShader) &&
			LoadShader("CubeMaptoConvolutedCubeMap.glsl", _convolutionShader) &&
			LoadShader("EnvironmentCubeMapShader.glsl", _cubeMapShader) &&
			LoadShader("PreFilteredCubeMapShader.glsl", _preFilteredShader) &&
			LoadShader("BRDFConvolutionShader.glsl", _brdfConvolutionShader) &&
			LoadUnitCube();

		if (_initialised)
		{
			_cubeMapShader->SetInt("u_EnvironmentMap", 0);

			const glm::vec2 aspectRatio = MimicCore::Window->GetAspectRatio();

			// set up framebuffer & renderbuffer object:
			if (_framebuffer == nullptr)
			{
				_framebuffer = Framebuffer::Initialise();
				_framebuffer->AttachRenderObject(GL_DEPTH_ATTACHMENT);
			}
			
			 glGenFramebuffers(1, &_framebufferId);
			 glGenRenderbuffers(1, &_renderObjectId);

			 glBindFramebuffer(GL_FRAMEBUFFER, _framebufferId);
			 glBindRenderbuffer(GL_RENDERBUFFER, _renderObjectId);
			 glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _renderObjectId);
			

			// hdr environment map:
			 _environmentMapTexture = MimicCore::ResourceManager->CreateResource<Texture>(aspectRatio, Texture::MIMIC_CUBEMAP_TEXTURE_PARAMS, Texture::MIMIC_RGB16F, Texture::MIMIC_RGB);
			LoadEnvironmentMap();

			// convolute cubemap:
			_irradianceMapTexture = MimicCore::ResourceManager->CreateResource<Texture>(glm::ivec2(32, 32), Texture::MIMIC_CUBEMAP_TEXTURE_PARAMS, Texture::MIMIC_RGB16F, Texture::MIMIC_RGB);
			LoadIrradianceMapTexture();

			// pre-filtred map:
			_prefilteredMapTexture = MimicCore::ResourceManager->CreateResource<Texture>(glm::ivec2(32, 32), Texture::MIMIC_CUBEMAP_TEXTURE_PARAMS | Texture::MIMIC_GEN_MIPMAP, Texture::MIMIC_RGB16F, Texture::MIMIC_RGB);
			LoadPrefilteredMapTexture();

			// BRDF convolution:
			_brdfConvolutedTexture = MimicCore::ResourceManager->CreateResource<Texture>(glm::ivec2(512, 512), Texture::MIMIC_BRDF_TEXTURE_PARAMS, Texture::MIMIC_RG16F, Texture::MIMIC_RG);
			LoadBRDFConvolutedTexture();

			glViewport(0, 0, aspectRatio.x, aspectRatio.y);
		}
	}

	void EnvironmentCubeMap::Draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
	{
		if (_skipDraw) return;
		if (!_initialised)
		{
			MIMIC_LOG_WARNING("[Mimic::EnvironmentCubeMap] Is uninitialised, therefore unable to draw.");
			_skipDraw = true;
		}

		glDepthFunc(GL_LEQUAL);
		_cubeMapShader->UseShader();
		_cubeMapShader->SetMat4("u_View", viewMatrix);
		_cubeMapShader->SetMat4("u_Projection", projectionMatrix);

		glBindVertexArray(_unitCubeVertexArrayId);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, _environmentMapTexture->_id);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glDepthFunc(GL_LESS);

		// render the BRDF 2D lookup texture to the screen:
		/*_brdfConvolutionShader->UseShader();
		RenderQuad();*/
	}

	const bool EnvironmentCubeMap::LoadEquirectangular(const std::string& fileName)
	{
		// check if the environment map has already been loaded:
		if (_equirectangularTexture != nullptr) 
		{
			if (_equirectangularTexture->Name == fileName) return true;
		}

		stbi_set_flip_vertically_on_load(true);
		_equirectangularTexture = MimicCore::ResourceManager->LoadResource<Texture>(fileName);

		return _equirectangularTexture != nullptr;
	}

	const bool EnvironmentCubeMap::LoadShader(const std::string& fileName, std::shared_ptr<Shader>& shader)
	{
		shader = MimicCore::ResourceManager->LoadResource<Shader>(fileName);
		return shader != nullptr;
	}

	const bool EnvironmentCubeMap::LoadUnitCube()
	{
		std::shared_ptr<Model> unitCube = MimicCore::ResourceManager->LoadResource<Model>("cube.obj");
		if (unitCube == nullptr)
		{
			MIMIC_LOG_WARNING("[Mimic::EnvironmentCubeMap] Unable to load cubemap unit cube with filename: \"%\"", "cube.obj");
			return false;
		}
		if (unitCube->_meshes.empty())
		{
			MIMIC_LOG_WARNING("[Mimic::EnvironmentCubeMap] Unable to load cubemap unit cube with filename: \"%\", the model contains no loaded meshes.", "cube.obj");
			return false;
		}
		_unitCubeVertexArrayId = unitCube->_meshes[0]->_vertexArrayId;

		return true;
	}

	void EnvironmentCubeMap::LoadEnvironmentMap()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _framebufferId);
		glBindRenderbuffer(GL_RENDERBUFFER, _renderObjectId);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, MimicCore::Window->GetAspectRatio().x, MimicCore::Window->GetAspectRatio().y);
		// _framebuffer->UtiliseRenderObject(GL_DEPTH_COMPONENT24, MimicCore::Window->GetAspectRatio());

		// convert HDR equirectangular environment map into an environment cubemap equivalent:
		_equirectangularToCubemapShader->UseShader();
		_equirectangularToCubemapShader->SetInt("u_EquirectangularMap", 1);
		_equirectangularToCubemapShader->SetMat4("u_Projection", _captureProjection);
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, _equirectangularTexture->_id);

		glBindFramebuffer(GL_FRAMEBUFFER, _framebufferId);
		for (unsigned int i = 0; i < 6; i++)
		{
			_equirectangularToCubemapShader->SetMat4("u_View", _captureViews[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, _environmentMapTexture->_id, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// render unit cube:
			glBindVertexArray(_unitCubeVertexArrayId);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void EnvironmentCubeMap::LoadIrradianceMapTexture()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _framebufferId);
		glBindRenderbuffer(GL_RENDERBUFFER, _renderObjectId);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);
		glViewport(0, 0, 32, 32);
		// _framebuffer->UtiliseRenderObject(GL_DEPTH_COMPONENT24, glm::ivec2(32, 32));

		// convert cubemap to irradiance map:
		_convolutionShader->UseShader();
		_convolutionShader->SetInt("u_EnvironmentMap", 1);
		_convolutionShader->SetMat4("u_Projection", _captureProjection);
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, _environmentMapTexture->_id);

		for (unsigned int i = 0; i < 6; i++)
		{
			_convolutionShader->SetMat4("u_View", _captureViews[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, _irradianceMapTexture->_id, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			RenderUnitCube();
		}
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void EnvironmentCubeMap::LoadPrefilteredMapTexture()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _framebufferId);

		// convert cubemap to pre-convoluted map:
		_preFilteredShader->UseShader();
		_preFilteredShader->SetInt("u_EnvironmentMap", 1);
		_preFilteredShader->SetMat4("u_Projection", _captureProjection);
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, _environmentMapTexture->_id);
	
		constexpr unsigned int maxMipLevels = 5;
		for (unsigned int mip = 0; mip < maxMipLevels; mip++)
		{
			// resize framebuffer according to mip-level size:
			const unsigned int mipWidth = 128 * std::pow(0.5f, mip);
			const unsigned int mipHeight = mipWidth;
			glBindRenderbuffer(GL_RENDERBUFFER, _renderObjectId);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
			glViewport(0, 0, mipWidth, mipHeight);

			float roughness = (float)mip / (float)(maxMipLevels - 1);
			_preFilteredShader->SetFloat("u_Roughness", roughness);

			for (unsigned int i = 0; i < 6; i++)
			{
				_preFilteredShader->SetMat4("u_View", _captureViews[i]);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, _prefilteredMapTexture->_id, mip);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				RenderUnitCube();
			}
		}
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void EnvironmentCubeMap::LoadBRDFConvolutedTexture()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _framebufferId);
		glBindRenderbuffer(GL_RENDERBUFFER, _renderObjectId);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _brdfConvolutedTexture->_id, 0);
		glViewport(0, 0, 512, 512);

		_brdfConvolutionShader->UseShader();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		RenderQuad();

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void EnvironmentCubeMap::RenderUnitCube() const noexcept
	{
		glBindVertexArray(_unitCubeVertexArrayId);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void EnvironmentCubeMap::RenderQuad()
	{
		if (_unitQuadVertexArrayId == 0)
		{
			unsigned int quadVBO;
			float quadVertices[] = {
				// positions        // texture Coords
				-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
				-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
				 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			};
			// setup plane VAO
			glGenVertexArrays(1, &_unitQuadVertexArrayId);
			glGenBuffers(1, &quadVBO);
			glBindVertexArray(_unitQuadVertexArrayId);
			glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		}
		glBindVertexArray(_unitQuadVertexArrayId);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}
}