#include "Material.h"
#include "MimicCore.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Shader.h"
#include "Texture.h"
#include "Light.h"
#include <mimic_render/RenderTexture.h>

#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace MimicEngine
{
	// #############################################################################
	// Material functions:
	// #############################################################################


	//// #############################################################################
	//// basic material functions:
	//// #############################################################################
	//BasicMaterial::BasicMaterial()
	//{
	//	_shader = MimicCore::ResourceManager->LoadResource<Shader>("BasicShader.glsl");
	//}

	//void BasicMaterial::OnDraw()
	//{
	//	if (_shader.expired()) return;
	//	const std::shared_ptr<Shader> shader = _shader.lock();
	//	if (!_diffuseTexture.expired()) shader->SetTexture("u_Diffuse", _diffuseTexture.lock()->_id, 0);
	//	if (!_specularTexture.expired()) shader->SetTexture("u_Specular", _specularTexture.lock()->_id, 1);
	//	if (!_normalTexture.expired()) shader->SetTexture("u_Normal", _normalTexture.lock()->_id, 2);
	//	if (!_heightTexture.expired()) shader->SetTexture("u_Height", _heightTexture.lock()->_id, 3);
	//}

	//void BasicMaterial::SetDiffuse(const std::shared_ptr<Texture>& diffuse)
	//{
	//	_diffuseTexture = diffuse;
	//}

	//void BasicMaterial::SetSpecular(const std::shared_ptr<Texture>& specular)
	//{
	//	_specularTexture = specular;
	//}

	//void BasicMaterial::SetNormal(const std::shared_ptr<Texture>& normal)
	//{
	//	_normalTexture = normal;
	//}

	//void BasicMaterial::SetHeight(const std::shared_ptr<Texture>& height)
	//{
	//	_heightTexture = height;
	//}

	//void BasicMaterial::SetTextureMap(const std::shared_ptr<Texture>& texture)
	//{
	//	const int type = texture->_type;

	//	if(type & TextureType::MIMIC_DIFFUSE) _diffuseTexture = texture;
	//	else if(type & TextureType::MIMIC_SPECULAR) _specularTexture = texture;
	//	else if (type & TextureType::MIMIC_HEIGHT) _heightTexture = texture;
	//	else if (type & TextureType::MIMIC_NORMAL) _normalTexture = texture;
	//	else MIMIC_LOG_WARNING("[Mimic::Material] Unable to set texture as it has no value type.");
	//}

	// #############################################################################
	// PBR Subroutine Helper Functions:
	// #############################################################################
	PBRSubroutineHelper::PBRSubroutineHelper(const unsigned int& uniformId, const unsigned int& autoTextureId, const unsigned int& manualTextureId)
		: _uniformId(uniformId), _autoId(autoTextureId), _manualId(manualTextureId)
	{

	}

	// #############################################################################
	// PBR Material Functions:
	// #############################################################################
	std::shared_ptr<PBRMaterial> PBRMaterial::Initialise()
	{
		auto pbrMaterial = std::make_shared<PBRMaterial>();

		// Load and assign shader:
		auto shaderEngine = MimicCore::ResourceManager->LoadResource<Shader>("PBRShader.glsl");
		auto shaderRender = shaderEngine->_renderShader;
		pbrMaterial->_shader = shaderEngine;

		// Source: https://stackoverflow.com/questions/23391311/glsl-subroutine-is-not-changed
		// Load and assign all subroutine uniforms, specifying which subroutine functions the shader should execute. 
		// 'AutoTexture' is used when a texture map of a specific type is initialised, whereas 'Manual' is 
		// used when no texture is present:
		pbrMaterial->_albedoSubroutine = PBRSubroutineHelper(
			shaderRender->GetSubroutineUniform(GL_FRAGMENT_SHADER, "AlbedoMode"),
			shaderRender->GetSubroutineIndex(GL_FRAGMENT_SHADER, "CalculateAlbedoAutoTexture"),
			shaderRender->GetSubroutineIndex(GL_FRAGMENT_SHADER, "CalculateAlbedoManual")
		);

		pbrMaterial->_normalSubroutine = PBRSubroutineHelper(
			shaderRender->GetSubroutineUniform(GL_FRAGMENT_SHADER, "NormalMode"),
			shaderRender->GetSubroutineIndex(GL_FRAGMENT_SHADER, "CalculateNormalAutoTexture"),
			shaderRender->GetSubroutineIndex(GL_FRAGMENT_SHADER, "CalculateNormalManual")
		);

		pbrMaterial->_roughnessSubroutine = PBRSubroutineHelper(
			shaderRender->GetSubroutineUniform(GL_FRAGMENT_SHADER, "RoughnessMode"),
			shaderRender->GetSubroutineIndex(GL_FRAGMENT_SHADER, "CalculateRoughnessAutoTexture"),
			shaderRender->GetSubroutineIndex(GL_FRAGMENT_SHADER, "CalculateRoughnessManual")
		);

		pbrMaterial->_metallicSubroutine = PBRSubroutineHelper(
			shaderRender->GetSubroutineUniform(GL_FRAGMENT_SHADER, "MetallicMode"),
			shaderRender->GetSubroutineIndex(GL_FRAGMENT_SHADER, "CalculateMetallicAutoTexture"),
			shaderRender->GetSubroutineIndex(GL_FRAGMENT_SHADER, "CalculateMetallicManual")
		);

		// Store all subroutine uniform IDs in an array:
		pbrMaterial->_subroutineIndices = {
			pbrMaterial->_albedoSubroutine._uniformId,
			pbrMaterial->_normalSubroutine._uniformId,
			pbrMaterial->_roughnessSubroutine._uniformId,
			pbrMaterial->_metallicSubroutine._uniformId
		};

		// Set PBR parameter defaults:
		pbrMaterial->SetAlbedo(glm::vec3(1.0f));
		pbrMaterial->SetEmissive(glm::vec3(0.0f));
		pbrMaterial->SetMetallic(0.0f);
		pbrMaterial->SetRoughness(0.9f);
		pbrMaterial->SetAmbientOcclusion(0.1f);
		pbrMaterial->SetAlpha(1.0f);

		return pbrMaterial;
	}

	void PBRMaterial::SetTextureMap(const std::shared_ptr<Texture>& texture, const MimicRender::TextureType& textureMapType)
	{
		if (texture == nullptr)
		{
			MIMIC_LOG_WARNING("[MimicEngine::PBRMaterial] Unable to set uninitialised texture.");
			return;
		}
		switch (textureMapType)
		{
		case MimicRender::TextureType::MIMIC_DIFFUSE: _albedoTexture = texture;
			break;
		case MimicRender::TextureType::MIMIC_SPECULAR: _metallicTexture = texture;
			break;
		case MimicRender::TextureType::MIMIC_NORMAL: _normalTexture = texture;
			break;
		case MimicRender::TextureType::MIMIC_ALBEDO: _albedoTexture = texture;
			break;
		case MimicRender::TextureType::MIMIC_ROUGHNESS: _roughnessTexture = texture;
			break;
		case MimicRender::TextureType::MIMIC_METALLIC: _metallicTexture = texture;
			break;
		default:
			break;
		}
	}

	void PBRMaterial::SetAlbedo(const glm::vec3& albedo)
	{
		_albedo = glm::clamp(albedo, 0.0f, 1.0f);
	}

	void PBRMaterial::SetEmissive(const glm::vec3& emissive)
	{
		_emissive = glm::clamp(emissive, 0.0f, 1.0f);
	}

	void PBRMaterial::SetMetallic(const float& metallic)
	{
		_metallic = std::clamp(metallic, 0.0f, 1.0f);
	}

	void PBRMaterial::SetRoughness(const float& roughness)
	{
		_roughness = std::clamp(roughness, 0.0f, 1.0f);
	}

	void PBRMaterial::SetAmbientOcclusion(const float& ambientOcclusion)
	{
		_ambientOcclusion = std::clamp(ambientOcclusion, 0.0f, 1.0f);
	}

	void PBRMaterial::SetAlpha(const float& alpha)
	{
		_alpha = std::clamp(alpha, 0.0f, 1.0f);
	}

	void PBRMaterial::OnDraw()
	{
		if (_shader.expired()) return;
		auto localShader = _shader.lock()->_renderShader;

		// For each texture map used in the PBR shader, check if the texture map is initialised, if not
		// set a literal PBR parameter instead:
	
		// Query albedo subroutine:
		if (!_albedoTexture.expired() && !NoTextureMode)
		{
			_subroutineIndices[_albedoSubroutine._uniformId] = _albedoSubroutine._autoId;
			localShader->SetTexture("u_AlbedoMap", _albedoTexture.lock()->GetId(), 0);
		}
		else
		{
			_subroutineIndices[_albedoSubroutine._uniformId] = _albedoSubroutine._manualId;
			localShader->SetVector3("u_Albedo", _albedo);
		}

		// Query roughness subroutine:
		if (!_roughnessTexture.expired() && !NoTextureMode)
		{
			_subroutineIndices[_roughnessSubroutine._uniformId] = _roughnessSubroutine._autoId;
			localShader->SetTexture("u_RoughnessMap", _roughnessTexture.lock()->GetId(), 1);
		}
		else
		{
			_subroutineIndices[_roughnessSubroutine._uniformId] = _roughnessSubroutine._autoId;
			localShader->SetFloat("u_Roughness", _roughness);
		}

		// Query normal subroutine:
		if (!_normalTexture.expired() && !NoTextureMode)
		{
			_subroutineIndices[_normalSubroutine._uniformId] = _normalSubroutine._autoId;
			localShader->SetTexture("u_NormalMap", _normalTexture.lock()->GetId(), 2);
		}
		else _subroutineIndices[_normalSubroutine._uniformId] = _normalSubroutine._manualId;

		// Query metallic subroutine:
		if (!_metallicTexture.expired() && !NoTextureMode)
		{
			_subroutineIndices[_metallicSubroutine._uniformId] = _metallicSubroutine._autoId;
			localShader->SetTexture("u_MetallicMap", _metallicTexture.lock()->GetId(), 3);
		}
		else
		{
			_subroutineIndices[_metallicSubroutine._uniformId] = _metallicSubroutine._manualId;
			localShader->SetFloat("u_Metallic", _metallic);
		}

		// Set texture units for all environment map data (used for image-based rendering):
		localShader->SetTexture("u_IrradianceMap", MimicCore::_environmentCubeMap->GetIrradianceId(), 4, MimicRender::Texture::MIMIC_CUBEMAP_TEXTURE);
		localShader->SetTexture("u_PrefilterMap", MimicCore::_environmentCubeMap->GetPreFilteredId(), 5, MimicRender::Texture::MIMIC_CUBEMAP_TEXTURE);
		localShader->SetTexture("u_BRDFLookupTexture", MimicCore::_environmentCubeMap->GetBRDFId(), 6, MimicRender::Texture::MIMIC_2D_TEXTURE);
		 
		// Send all subroutine selection data to the GPU, telling the shader which subroutines to query:
		glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, _subroutineIndices.size(), _subroutineIndices.data());

		// Set remaining PBR literal parameters that are not tied to texture maps:
		localShader->SetVector3("u_Emissive", _emissive);
		localShader->SetFloat("u_Alpha", _alpha);
		localShader->SetFloat("u_AmbientOcclusion", _ambientOcclusion);

		// Set uniforms for each direct light:
		int index = 0;
		for (auto directLight : MimicCore::_directLights)
		{
			// Assign a new direct light into an array within the PBR shader, along with the
			// light's attributes (direction & colour):
			const std::string lightUniformId = "u_DirectLights[" + std::to_string(index) + "]";
			localShader->SetVector3((lightUniformId + ".direction").c_str(), glm::normalize(-directLight->GetDirection()));
			localShader->SetVector4((lightUniformId + ".colour").c_str(), glm::vec4(directLight->GetColour(), _alpha));

			// Assign depth map for this direct light:
			const std::string currentShadowMap = "u_DirectShadowMaps[" + std::to_string(index) + "]";
			localShader->SetTexture(currentShadowMap.c_str(), directLight->_renderDirectLight->GetDepthMapTextureId(), 7, MimicRender::Texture::MIMIC_2D_TEXTURE);

			const std::string currentLightMatrix = "u_DirectLightMatrices[" + std::to_string(index) + "]";
			localShader->SetMat4(currentLightMatrix.c_str(), directLight->_renderDirectLight->GetLightMatrix());
			index++;
		}
		localShader->SetInt("u_DirectLightsCount", MimicCore::_directLights.size());

		//// Set uniforms for each point light:
		//const std::vector<std::shared_ptr<PointLight>> pointLights = MimicCore::_pointLights;
		//for (int i = 0; i < pointLights.size(); i++)
		//{
		//	const std::string currentLight = "u_PointLights[" + std::to_string(i) + "]";

		//	localShader->SetVector3((currentLight + ".position").c_str(), pointLights[i]->Position);
		//	const glm::vec4 colour = glm::vec4(pointLights[i]->Colour.x, pointLights[i]->Colour.y, pointLights[i]->Colour.z, 1.0f);
		//	localShader->SetVector4((currentLight + ".colour").c_str(), colour);
		//	localShader->SetFloat((currentLight + ".constant").c_str(), pointLights[i]->Constant);
		//	localShader->SetFloat((currentLight + ".linear").c_str(), pointLights[i]->Linear);
		//	localShader->SetFloat((currentLight + ".quadratic").c_str(), pointLights[i]->Quadratic);
		//}
		//localShader->SetInt("u_PointLightsCount", pointLights.size());
	}

	//// #############################################################################
	//// cubemap material functions:
	//// #############################################################################
	//CubeMapMaterial::CubeMapMaterial()
	//{

	//}

	//void CubeMapMaterial::SetSourceTexture(const std::shared_ptr<Texture>& texture)
	//{
	//	if (texture == nullptr)
	//	{
	//		MIMIC_LOG_WARNING("[Mimic::Material] Unable to set unitialised texture.");
	//		return;
	//	}
	//	_sourceTexture = texture;
	//}

	//void CubeMapMaterial::OnDraw()
	//{

	//}
}