#include "Material.h"
#include "MimicCore.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Shader.h"
#include "Texture.h"
#include <mimic_render/RenderTexture.h>
// #include <mimic/Light.h>

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
	// PBR Material functions:
	// #############################################################################
	std::shared_ptr<PBRMaterial> PBRMaterial::Initialise()
	{
		auto pbrMaterial = std::shared_ptr<PBRMaterial>();

		auto shader = MimicCore::ResourceManager->LoadResource<Shader>("PBRShader.glsl");

		// Source: https://stackoverflow.com/questions/23391311/glsl-subroutine-is-not-changed
		// Load all subroutines that control how to load certain PBR parameters. 'AutoTexture' is used when 
		// a texture of a specific type is present, whereas 'Manual' is used when no texture is present:
		pbrMaterial->_albedoSubroutineUniform = glGetSubroutineUniformLocation(shader->GetShaderId(), GL_FRAGMENT_SHADER, "AlbedoMode");
		pbrMaterial->_autoAlbedo = glGetSubroutineIndex(shader->GetShaderId(), GL_FRAGMENT_SHADER, "CalculateAlbedoAutoTexture");
		pbrMaterial->_manualAlbedo = glGetSubroutineIndex(shader->GetShaderId(), GL_FRAGMENT_SHADER, "CalculateAlbedoManual");

		pbrMaterial->_normalSubroutineUniform = glGetSubroutineUniformLocation(shader->GetShaderId(), GL_FRAGMENT_SHADER, "NormalMode");
		pbrMaterial->_autoNormal = glGetSubroutineIndex(shader->GetShaderId(), GL_FRAGMENT_SHADER, "CalculateNormalAutoTexture");
		pbrMaterial->_manualNormal = glGetSubroutineIndex(shader->GetShaderId(), GL_FRAGMENT_SHADER, "CalculateNormalManual");

		pbrMaterial->_roughnessSubroutineUniform = glGetSubroutineUniformLocation(shader->GetShaderId(), GL_FRAGMENT_SHADER, "RoughnessMode");
		pbrMaterial->_autoRoughness = glGetSubroutineIndex(shader->GetShaderId(), GL_FRAGMENT_SHADER, "CalculateRoughnessAutoTexture");
		pbrMaterial->_manualRoughness = glGetSubroutineIndex(shader->GetShaderId(), GL_FRAGMENT_SHADER, "CalculateRoughnessManual");

		pbrMaterial->_metallicSubroutineUniform = glGetSubroutineUniformLocation(shader->GetShaderId(), GL_FRAGMENT_SHADER, "MetallicMode");
		pbrMaterial->_autoMetallic = glGetSubroutineIndex(shader->GetShaderId(), GL_FRAGMENT_SHADER, "CalculateMetallicAutoTexture");
		pbrMaterial->_manualMetallic = glGetSubroutineIndex(shader->GetShaderId(), GL_FRAGMENT_SHADER, "CalculateMetallicManual");

		pbrMaterial->_subroutineIndices = { pbrMaterial->_albedoSubroutineUniform, pbrMaterial->_normalSubroutineUniform, pbrMaterial->_roughnessSubroutineUniform, pbrMaterial->_metallicSubroutineUniform };

		// Assign shader:
		pbrMaterial->_shader = shader;

		// Set PBR parameters to defaults:
		pbrMaterial->SetAlbedo(glm::vec3(1.0f, 0.0f, 0.0f));
		pbrMaterial->SetEmissive(glm::vec3(0.0f));
		pbrMaterial->SetMetallic(0.0f);
		pbrMaterial->SetRoughness(0.5f);
		pbrMaterial->SetAmbientOcclusion(0.5f);
		pbrMaterial->SetAlpha(1.0f);

		return pbrMaterial;
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
		_metallic = std::clamp(metallic, 0.001f, 1.0f);
	}

	void PBRMaterial::SetRoughness(const float& roughness)
	{
		_roughness = std::clamp(roughness, 0.001f, 1.0f);
	}

	void PBRMaterial::SetAmbientOcclusion(const float& ambientOcclusion)
	{
		_ambientOcclusion = std::clamp(ambientOcclusion, 0.0f, 1.0f);
	}

	void PBRMaterial::SetAlpha(const float& alpha)
	{
		_alpha = std::clamp(alpha, 0.0f, 1.0f);
	}

	///*void PBRMaterial::SetTextureMap(const std::shared_ptr<Texture>& texture)
	//{
	//	const int type = texture->_type;

	//	if (type & TextureType::MIMIC_DIFFUSE || type & TextureType::MIMIC_ALBEDO) _albedoTexture = texture;
	//	else if (type & TextureType::MIMIC_SPECULAR || type & TextureType::MIMIC_ROUGHNESS) _roughnessTexture = texture;
	//	else if (type & TextureType::MIMIC_METALLIC) _metallicTexture = texture;
	//	else if (type & TextureType::MIMIC_NORMAL) _normalTexture = texture;
	//	else MIMIC_LOG_WARNING("[Mimic::Material] Unable to set texture as it has no value type.");
	//};*/

	//void PBRMaterial::OnDraw()
	//{
	//	if (_shader.expired()) return;
	//	const std::shared_ptr<Shader> shader = _shader.lock();

	//	// load albedo (map_kd):
	//	if (!_albedoTexture.expired() && !ManualMode)
	//	{
	//		_subroutineIndices[_albedoSubroutineUniform] = _autoAlbedo;
	//		shader->SetTexture("u_AlbedoMap", _albedoTexture.lock()->_id, 1); // texture unit slots start at 1.
	//	}
	//	else
	//	{
	//		_subroutineIndices[_albedoSubroutineUniform] = _manualAlbedo;
	//		shader->SetVector3("u_Albedo", Albedo);
	//	}

	//	// load roughness(map_ks):
	//	if (!_roughnessTexture.expired() && !ManualMode)
	//	{
	//		_subroutineIndices[_roughnessSubroutineUniform] = _autoRoughness;
	//		shader->SetTexture("u_RoughnessMap", _roughnessTexture.lock()->_id, 2);
	//	}
	//	else
	//	{
	//		_subroutineIndices[_roughnessSubroutineUniform] = _manualRoughness;
	//		shader->SetFloat("u_Roughness", Roughness);
	//	}


	//	// load normals (map_Bump):
	//	if (!_normalTexture.expired() && !ManualMode)
	//	{
	//		_subroutineIndices[_normalSubroutineUniform] = _autoNormal;
	//		shader->SetTexture("u_NormalMap", _normalTexture.lock()->_id, 3);
	//	}
	//	else _subroutineIndices[_normalSubroutineUniform] = _manualNormal;

	//	// load metallic (must be specified by user):
	//	if (!_metallicTexture.expired() && !ManualMode)
	//	{
	//		_subroutineIndices[_metallicSubroutineUniform] = _autoMetallic;
	//		shader->SetTexture("u_MetallicMap", _metallicTexture.lock()->_id, 4);
	//	}
	//	else
	//	{
	//		_subroutineIndices[_metallicSubroutineUniform] = _manualMetallic;
	//		shader->SetFloat("u_Metallic", Metallic);
	//	}

	//	glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, _subroutineIndices.size(), _subroutineIndices.data());

	//	shader->SetInt("u_IrradianceMap", 5);
	//	glActiveTexture(GL_TEXTURE5);
	//	glBindTexture(GL_TEXTURE_CUBE_MAP, MimicCore::EnvironmentCubeMap->_irradianceRenderTexture->_texture->_id);

	//	shader->SetInt("u_PrefilterMap", 6);
	//	glActiveTexture(GL_TEXTURE6);
	//	glBindTexture(GL_TEXTURE_CUBE_MAP, MimicCore::EnvironmentCubeMap->_prefilteredMapRenderTexture->_texture->_id);

	//	shader->SetInt("u_BRDFLookupTexture", 7);
	//	glActiveTexture(GL_TEXTURE7);
	//	glBindTexture(GL_TEXTURE_2D, MimicCore::EnvironmentCubeMap->_brdfConvolutedRenderTexture->_texture->_id);
	//	
	//	shader->SetVector3("u_Emissive", Emissive);
	//	shader->SetFloat("u_Alpha", Alpha);
	//	shader->SetFloat("u_AmbientOcclusion", AmbientOcclusion);

	//	// direct lights:
	//	const std::vector<std::shared_ptr<DirectLight>> directLights = MimicCore::_directLights;
	//	for (int i = 0; i < directLights.size(); i++)
	//	{
	//		const std::string currentLight = "u_DirectLights[" + std::to_string(i) + "]";

	//		shader->SetVector3((currentLight + ".direction").c_str(), glm::normalize(-directLights[i]->Direction));
	//		const glm::vec4 colour = glm::vec4(directLights[i]->Colour.x, directLights[i]->Colour.y, directLights[i]->Colour.z, 1.0f);
	//		shader->SetVector4((currentLight + ".colour").c_str(), colour);
	//	}
	//	shader->SetInt("u_DirectLightsCount", directLights.size());


	//	// point lights:
	//	const std::vector<std::shared_ptr<PointLight>> pointLights = MimicCore::_pointLights;
	//	for (int i = 0; i < pointLights.size(); i++)
	//	{
	//		const std::string currentLight = "u_PointLights[" + std::to_string(i) + "]";

	//		shader->SetVector3((currentLight + ".position").c_str(), pointLights[i]->Position);
	//		const glm::vec4 colour = glm::vec4(pointLights[i]->Colour.x, pointLights[i]->Colour.y, pointLights[i]->Colour.z, 1.0f);
	//		shader->SetVector4((currentLight + ".colour").c_str(), colour);
	//		shader->SetFloat((currentLight + ".constant").c_str(), pointLights[i]->Constant);
	//		shader->SetFloat((currentLight + ".linear").c_str(), pointLights[i]->Linear);
	//		shader->SetFloat((currentLight + ".quadratic").c_str(), pointLights[i]->Quadratic);
	//	}
	//	shader->SetInt("u_PointLightsCount", pointLights.size());
	//}

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