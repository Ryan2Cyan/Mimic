#include "CubeMap.h"
#include <utility/Logger.h>
#include <utility/FileLoader.h>
#include <renderengine/Shader.h>
#include <renderengine/Model.h>
#include <renderengine/RenderTexture.h>
#include <renderengine/Renderer.h>
#include <renderengine/Window.h>
#include <stb_image.h>
#include <GL/glew.h>
#include <functional>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>

namespace MimicRender
{
	//// #############################################################################
	//// hdr cubemap functions:
	//// #############################################################################

	const std::shared_ptr<EnvironmentCubeMap> EnvironmentCubeMap::Initialise(const std::string& hdrFileName, const glm::vec2& aspectRatio, std::shared_ptr<Renderer>& renderer)
	{
		std::shared_ptr<EnvironmentCubeMap> environementMap = std::make_shared<EnvironmentCubeMap>();
		environementMap->Load(hdrFileName, aspectRatio, renderer);
		return environementMap;
	}

	void EnvironmentCubeMap::Load(const std::string& equirectangularTextureFileName, const glm::vec2& aspectRatio, std::shared_ptr<Renderer>& renderer)
	{
		_captureProjection = glm::mat4(1.0f);
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
		// file loader:
		std::shared_ptr<Mimic::FileLoader> fileLoader = Mimic::FileLoader::Initialise();
		const std::string assetsDirectory = fileLoader->LocateDirectory("assets").generic_string();

		// load hdr texture:
		stbi_set_flip_vertically_on_load(true);
		_equirectangularTexture = Texture::Initialise(fileLoader->LocateFileInDirectory(assetsDirectory, equirectangularTextureFileName), aspectRatio, Texture::MIMIC_2D_TEXTURE_PARAMS);
		stbi_set_flip_vertically_on_load(false);

		// load shaders:
		_equirectangularToCubemapShader = Shader::Initialise(fileLoader->LocateFileInDirectory(assetsDirectory, "EquirectangularToCubemapShader.glsl"));
		_convolutionShader = Shader::Initialise(fileLoader->LocateFileInDirectory(assetsDirectory, "CubeMaptoConvolutedCubeMap.glsl"));
		_preFilteredShader = Shader::Initialise(fileLoader->LocateFileInDirectory(assetsDirectory, "PreFilteredCubeMapShader.glsl"));
		_brdfConvolutionShader = Shader::Initialise(fileLoader->LocateFileInDirectory(assetsDirectory, "BRDFConvolutionShader.glsl"));
		_cubeMapShader = Shader::Initialise(fileLoader->LocateFileInDirectory(assetsDirectory, "EnvironmentCubeMapShader.glsl"));

		_cubeMapShader->SetInt("u_EnvironmentMap", 0);

		// environment map render texture:
		if (_environmentMapRenderTexture == nullptr)
		{
			_environmentMapRenderTexture = RenderTexture::Initialise();
			_environmentMapRenderTexture->SetTexture(Texture::Initialise(glm::ivec2(512, 512), Texture::MIMIC_CUBEMAP_TEXTURE_PARAMS, Texture::MIMIC_RGB16F, Texture::MIMIC_RGB));
		}
		LoadEnvironmentMap(renderer);

		// irradiance map render texture:
		if (_irradianceRenderTexture == nullptr)
		{
			_irradianceRenderTexture = RenderTexture::Initialise();
			_irradianceRenderTexture->SetTexture(Texture::Initialise(glm::ivec2(32, 32), Texture::MIMIC_CUBEMAP_TEXTURE_PARAMS, Texture::MIMIC_RGB16F, Texture::MIMIC_RGB));
		}
		LoadIrradianceMapTexture(renderer);

		// prefiltered map render texture:
		if (_prefilteredMapRenderTexture == nullptr)
		{
			_prefilteredMapRenderTexture = RenderTexture::Initialise();
			_prefilteredMapRenderTexture->SetTexture(Texture::Initialise(glm::ivec2(32, 32), Texture::MIMIC_CUBEMAP_TEXTURE_PARAMS, Texture::MIMIC_RGB16F, Texture::MIMIC_RGB));
		}
		LoadPrefilteredMapTexture(renderer);

		// brdf map render texture:
		if (_brdfConvolutedRenderTexture == nullptr)
		{
			_brdfConvolutedRenderTexture = RenderTexture::Initialise();
			_brdfConvolutedRenderTexture->SetTexture(Texture::Initialise(glm::ivec2(512, 512), Texture::MIMIC_BRDF_TEXTURE_PARAMS, Texture::MIMIC_RG16F, Texture::MIMIC_RG));
		}
		LoadBRDFConvolutedTexture(renderer);

		glViewport(0, 0, aspectRatio.x, aspectRatio.y);
	}

	const unsigned int EnvironmentCubeMap::GetIrradianceId() const
	{
		return _irradianceRenderTexture->_texture->_id;
	}

	const unsigned int EnvironmentCubeMap::GetPreFilteredId() const
	{
		return _prefilteredMapRenderTexture->_texture->_id;
	}

	const unsigned int EnvironmentCubeMap::GetBRDFId() const
	{
		return _brdfConvolutedRenderTexture->_texture->_id;
	}

	void EnvironmentCubeMap::Draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, std::shared_ptr<Renderer>& renderer)
	{
		glDepthFunc(GL_LEQUAL);
		_cubeMapShader->UseShader();
		_cubeMapShader->SetMat4("u_View", viewMatrix);
		_cubeMapShader->SetMat4("u_Projection", projectionMatrix);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, _environmentMapRenderTexture->_texture->_id);
		renderer->DrawUnitCube();
		glBindVertexArray(0);

		glDepthFunc(GL_LESS);

		// render the BRDF 2D lookup texture to the screen:
		/*_brdfConvolutionShader->UseShader();
		RenderQuad();*/
	}

	void EnvironmentCubeMap::LoadEnvironmentMap(std::shared_ptr<Renderer>& renderer)
	{
		std::function<void()> onDrawLambda = [&]()
		{
			_equirectangularToCubemapShader->SetInt("u_EquirectangularMap", 1);
			glActiveTexture(GL_TEXTURE0 + 1);
			glBindTexture(GL_TEXTURE_2D, _equirectangularTexture->_id);
		};

		renderer->CaptureCubeMap
		(
			onDrawLambda,
			_equirectangularToCubemapShader,
			_environmentMapRenderTexture,
			glm::ivec2(512, 512)
		);
	}

	void EnvironmentCubeMap::LoadIrradianceMapTexture(std::shared_ptr<Renderer>& renderer)
	{
		std::function<void()> onDrawLambda = [&]()
		{
			_convolutionShader->SetInt("u_EnvironmentMap", 1);
			glActiveTexture(GL_TEXTURE0 + 1);
			glBindTexture(GL_TEXTURE_CUBE_MAP, _environmentMapRenderTexture->_texture->_id);
		};

		renderer->CaptureCubeMap
		(
			onDrawLambda,
			_convolutionShader,
			_irradianceRenderTexture,
			glm::ivec2(32, 32)
		);
	}

	void EnvironmentCubeMap::LoadPrefilteredMapTexture(std::shared_ptr<Renderer>& renderer)
	{
		_prefilteredMapRenderTexture->Bind();

		// convert cubemap to pre-convoluted map:
		_preFilteredShader->UseShader();
		_preFilteredShader->SetInt("u_EnvironmentMap", 1);
		_preFilteredShader->SetMat4("u_Projection", _captureProjection);
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, _environmentMapRenderTexture->_texture->_id);

		constexpr int startTargetIndex = (int)TextureTarget::MIMIC_CUBE_MAP_POSITIVE_X;
		constexpr unsigned int maxMipLevels = 5;
		for (unsigned int mip = 0; mip < maxMipLevels; mip++)
		{
			// resize framebuffer according to mip-level size:
			const unsigned int mipWidth = 128 * std::pow(0.5f, mip);
			const unsigned int mipHeight = mipWidth;

			_prefilteredMapRenderTexture->UseRenderObject(glm::ivec2(mipWidth, mipHeight));

			float roughness = (float)mip / (float)(maxMipLevels - 1);
			_preFilteredShader->SetFloat("u_Roughness", roughness);

			for (unsigned int i = 0; i < 6; i++)
			{
				_preFilteredShader->SetMat4("u_View", _captureViews[i]);
				_prefilteredMapRenderTexture->BindTextureForRender((TextureTarget)(startTargetIndex + i), mip);
				renderer->DrawUnitCube();
			}
		}
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		_prefilteredMapRenderTexture->Unbind();
	}

	void EnvironmentCubeMap::LoadBRDFConvolutedTexture(std::shared_ptr<Renderer>& renderer)
	{
		_brdfConvolutedRenderTexture->UseRenderObject(glm::ivec2(512, 512));
		_brdfConvolutedRenderTexture->BindTextureForRender(TextureTarget::MIMIC_TEXTURE_2D);

		_brdfConvolutionShader->UseShader();

		renderer->DrawUnitQuad();

		glBindTexture(GL_TEXTURE_2D, 0);
		_brdfConvolutedRenderTexture->Unbind();
	}
}