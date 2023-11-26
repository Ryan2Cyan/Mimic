#include "CubeMap.h"
#include <utility/Logger.h>
#include <utility/FileLoader.h>
#include <renderengine/Shader.h>
#include <renderengine/RenderTexture.h>
#include <renderengine/Renderer.h>
#include <stb_image.h>
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
		// initialise capture views and projection matrices:
		const glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
		const std::array<glm::mat4, 6> captureViews =
		{
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
		};

		// initialise file loader:
		std::shared_ptr<Mimic::FileLoader> fileLoader = Mimic::FileLoader::Initialise();
		const std::string assetsDirectory = fileLoader->LocateDirectory("assets").generic_string();

		// load hdr texture:
		const std::shared_ptr<Texture> equirectangularTexture = Texture::Initialise(fileLoader->LocateFileInDirectory(assetsDirectory, hdrFileName), aspectRatio, Texture::MIMIC_2D_TEXTURE_PARAMS | Texture::MIMIC_FLIP_VERTICAL);
		if (equirectangularTexture == nullptr)
		{
			MIMIC_LOG_WARNING("[MimicRender::EnvironmentCubeMap] Unable to initialise cubemap with hdr file from path \"%\"", hdrFileName);
			return environementMap;
		}

		// initialise shaders:
		const std::shared_ptr<Shader> equirectangularToCubemapShader = Shader::Initialise(fileLoader->LocateFileInDirectory(assetsDirectory, "EquirectangularToCubemapShader.glsl"));
		const std::shared_ptr<Shader> convolutionShader = Shader::Initialise(fileLoader->LocateFileInDirectory(assetsDirectory, "CubeMaptoConvolutedCubeMap.glsl"));
		const std::shared_ptr<Shader> preFilteredShader = Shader::Initialise(fileLoader->LocateFileInDirectory(assetsDirectory, "PreFilteredCubeMapShader.glsl"));
		const std::shared_ptr<Shader> brdfConvolutionShader = Shader::Initialise(fileLoader->LocateFileInDirectory(assetsDirectory, "BRDFConvolutionShader.glsl"));
		const std::shared_ptr<Shader> cubeMapShader = Shader::Initialise(fileLoader->LocateFileInDirectory(assetsDirectory, "EnvironmentCubeMapShader.glsl"));
		cubeMapShader->SetInt("u_EnvironmentMap", 0);

		// initialise render textures:
		std::shared_ptr<RenderTexture> environmentMapRenderTexture = RenderTexture::Initialise();
		environmentMapRenderTexture->SetTexture(Texture::Initialise(glm::ivec2(512, 512), Texture::MIMIC_CUBEMAP_TEXTURE_PARAMS, Texture::MIMIC_RGB16F, Texture::MIMIC_RGB));
		std::shared_ptr<RenderTexture> irradianceRenderTexture = RenderTexture::Initialise();
		irradianceRenderTexture->SetTexture(Texture::Initialise(glm::ivec2(32, 32), Texture::MIMIC_CUBEMAP_TEXTURE_PARAMS, Texture::MIMIC_RGB16F, Texture::MIMIC_RGB));
		std::shared_ptr<RenderTexture> prefilteredMapRenderTexture = RenderTexture::Initialise();
		prefilteredMapRenderTexture->SetTexture(Texture::Initialise(glm::ivec2(32, 32), Texture::MIMIC_CUBEMAP_TEXTURE_PARAMS, Texture::MIMIC_RGB16F, Texture::MIMIC_RGB));
		std::shared_ptr<RenderTexture> brdfConvolutedRenderTexture = RenderTexture::Initialise();
		brdfConvolutedRenderTexture->SetTexture(Texture::Initialise(glm::ivec2(512, 512), Texture::MIMIC_BRDF_TEXTURE_PARAMS, Texture::MIMIC_RG16F, Texture::MIMIC_RG));
		
		// initialise environment map:
		std::function<void()> onDrawLambda = [&]()
		{
			equirectangularToCubemapShader->SetInt("u_EquirectangularMap", 1);
			glActiveTexture(GL_TEXTURE0 + 1);
			glBindTexture(GL_TEXTURE_2D, equirectangularTexture->_id);
		};
		CaptureCubeMap(onDrawLambda, equirectangularToCubemapShader, environmentMapRenderTexture, glm::ivec2(512, 512), renderer, captureProjection, captureViews);

		// initialise irradiance map:
		onDrawLambda = [&]()
		{
			convolutionShader->SetInt("u_EnvironmentMap", 1);
			glActiveTexture(GL_TEXTURE0 + 1);
			glBindTexture(GL_TEXTURE_CUBE_MAP, environmentMapRenderTexture->_texture->_id);
		};
		CaptureCubeMap(onDrawLambda, convolutionShader, irradianceRenderTexture, glm::ivec2(32, 32), renderer, captureProjection, captureViews);

		// initialise pre-filtered map:
		prefilteredMapRenderTexture->Bind();
		preFilteredShader->UseShader();
		preFilteredShader->SetInt("u_EnvironmentMap", 1);
		preFilteredShader->SetMat4("u_Projection", captureProjection);
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, environmentMapRenderTexture->_texture->_id);

		constexpr int startTargetIndex = (int)TextureTarget::MIMIC_CUBE_MAP_POSITIVE_X;
		constexpr unsigned int maxMipLevels = 5;
		for (unsigned int mip = 0; mip < maxMipLevels; mip++)
		{
			// resize framebuffer according to mip-level size:
			const unsigned int mipWidth = 128 * std::pow(0.5f, mip);
			const unsigned int mipHeight = mipWidth;

			prefilteredMapRenderTexture->UseRenderObject(glm::ivec2(mipWidth, mipHeight));

			float roughness = (float)mip / (float)(maxMipLevels - 1);
			preFilteredShader->SetFloat("u_Roughness", roughness);

			for (unsigned int i = 0; i < 6; i++)
			{
				preFilteredShader->SetMat4("u_View", captureViews[i]);
				prefilteredMapRenderTexture->BindTextureForRender((TextureTarget)(startTargetIndex + i), mip);
				renderer->DrawUnitCube();
			}
		}
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		prefilteredMapRenderTexture->Unbind();

		// initialise 2D BRDF roughness texture:
		brdfConvolutedRenderTexture->UseRenderObject(glm::ivec2(512, 512));
		brdfConvolutedRenderTexture->BindTextureForRender(TextureTarget::MIMIC_TEXTURE_2D);
		brdfConvolutionShader->UseShader();
		renderer->DrawUnitQuad();
		glBindTexture(GL_TEXTURE_2D, 0);
		brdfConvolutedRenderTexture->Unbind();

		glViewport(0, 0, aspectRatio.x, aspectRatio.y);

		// assign values:
		environementMap->_irradianceRenderTexture = irradianceRenderTexture;
		environementMap->_prefilteredMapRenderTexture = prefilteredMapRenderTexture;
		environementMap->_brdfConvolutedRenderTexture = brdfConvolutedRenderTexture;
		environementMap->_environmentMapRenderTexture = environmentMapRenderTexture;
		environementMap->_cubeMapShader = cubeMapShader;
		environementMap->_initialised = true;
		return environementMap;
	}

	void EnvironmentCubeMap::CaptureCubeMap(std::function<void()>& onDrawLambda, const std::shared_ptr<Shader>& shader, std::shared_ptr<RenderTexture>& renderTexture,
		const glm::ivec2& aspectRatio, std::shared_ptr<Renderer>& renderer, const glm::mat4& captureProjection, const std::array<glm::mat4, 6>& captureViews)
	{
		renderTexture->UseRenderObject(aspectRatio);
		shader->UseShader();
		shader->SetMat4("u_Projection", captureProjection);
		onDrawLambda();

		constexpr int startTargetIndex = (int)TextureTarget::MIMIC_CUBE_MAP_POSITIVE_X;
		for (unsigned int i = 0; i < 6; i++)
		{
			shader->SetMat4("u_View", captureViews[i]);
			renderTexture->BindTextureForRender((TextureTarget)(startTargetIndex + i));

			// render unit cube:
			renderer->DrawUnitCube();
		}
		renderTexture->Unbind();
	}

	const unsigned int EnvironmentCubeMap::GetIrradianceId() const
	{
		if (!_initialised) return 0;
		return _irradianceRenderTexture->_texture->_id;
	}

	const unsigned int EnvironmentCubeMap::GetPreFilteredId() const
	{
		if (!_initialised) return 0;
		return _prefilteredMapRenderTexture->_texture->_id;
	}

	const unsigned int EnvironmentCubeMap::GetBRDFId() const
	{
		if (!_initialised) return 0;
		return _brdfConvolutedRenderTexture->_texture->_id;
	}
}