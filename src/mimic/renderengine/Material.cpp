#include "Material.h"
#include <lowlevelsystems/MimicCore.h>
#include <lowlevelsystems/ResourceManager.h>
#include <lowlevelsystems/GameObject.h>
#include <lowlevelsystems/CubeMap.h>
#include <renderengine/Shader.h>
#include <renderengine/Texture.h>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Mimic
{
	// #############################################################################
	// basic material functions:
	// #############################################################################
	void Material::SetShader(const std::shared_ptr<Shader>& shader)
	{
		_shader = shader;
	}

	void Material::SetDiffuse(const std::shared_ptr<Texture>& diffuse)
	{
		_diffuseTexture = diffuse;
	}

	void Material::SetSpecular(const std::shared_ptr<Texture>& specular)
	{
		_specularTexture = specular;
	}

	void Material::SetNormal(const std::shared_ptr<Texture>& normal)
	{
		_normalTexture = normal;
	}

	void Material::SetHeight(const std::shared_ptr<Texture>& height)
	{
		_heightTexture = height;
	}

	// #############################################################################
	// basic material functions:
	// #############################################################################

	BasicMaterial::BasicMaterial()
	{
		_shader = MimicCore::ResourceManager->LoadResource<Shader>("BasicShader.glsl");
	}

	void BasicMaterial::OnDraw()
	{
		if (_shader.expired()) return;
		const std::shared_ptr<Shader> shader = _shader.lock();
		if (!_diffuseTexture.expired()) shader->SetTexture("u_Diffuse", _diffuseTexture.lock()->_id, 0);
		if (!_specularTexture.expired()) shader->SetTexture("u_Specular", _specularTexture.lock()->_id, 1);
		if (!_normalTexture.expired()) shader->SetTexture("u_Normal", _normalTexture.lock()->_id, 2);
		if (!_heightTexture.expired()) shader->SetTexture("u_Height", _heightTexture.lock()->_id, 3);
	}

	// #############################################################################
	// pbr functions:
	// #############################################################################

	PBRMaterial::PBRMaterial()
	{
		std::shared_ptr<Shader> shader = MimicCore::ResourceManager->LoadResource<Shader>("PBRShader.glsl");

		ManualMode = false;

		// load all subroutines:
	    // Source: https://stackoverflow.com/questions/23391311/glsl-subroutine-is-not-changed
		_albedoSubroutineUniform = glGetSubroutineUniformLocation(shader->_shaderProgramId, GL_FRAGMENT_SHADER, "AlbedoMode");
		_autoAlbedo = glGetSubroutineIndex(shader->_shaderProgramId, GL_FRAGMENT_SHADER, "CalculateAlbedoAutoTexture");
		_manualAlbedo = glGetSubroutineIndex(shader->_shaderProgramId, GL_FRAGMENT_SHADER, "CalculateAlbedoManual");

		_normalSubroutineUniform = glGetSubroutineUniformLocation(shader->_shaderProgramId, GL_FRAGMENT_SHADER, "NormalMode");
		_autoNormal = glGetSubroutineIndex(shader->_shaderProgramId, GL_FRAGMENT_SHADER, "CalculateNormalAutoTexture");
		_manualNormal = glGetSubroutineIndex(shader->_shaderProgramId, GL_FRAGMENT_SHADER, "CalculateNormalManual");

		_roughnessSubroutineUniform = glGetSubroutineUniformLocation(shader->_shaderProgramId, GL_FRAGMENT_SHADER, "RoughnessMode");
		_autoRoughness = glGetSubroutineIndex(shader->_shaderProgramId, GL_FRAGMENT_SHADER, "CalculateRoughnessAutoTexture");
		_manualRoughness = glGetSubroutineIndex(shader->_shaderProgramId, GL_FRAGMENT_SHADER, "CalculateRoughnessManual");

		_subroutineIndices = { _albedoSubroutineUniform, _normalSubroutineUniform, _roughnessSubroutineUniform };

		SetAlbedo(glm::vec3(1.0f));
		SetEmissive(glm::vec3(0.0f));
		SetMetallic(0.0f);
		SetRoughness(0.5f);
		SetAmbientOcclusion(0.001f);
		SetAlpha(1.0f);

		_shader = shader;
	}

	void PBRMaterial::SetAlbedo(const glm::vec3& albedo)
	{
		Albedo = glm::clamp(albedo, 0.0f, 1.0f);
	}

	void PBRMaterial::SetEmissive(const glm::vec3& emissive)
	{
		Emissive = glm::clamp(emissive, 0.0f, 1.0f);
	}

	void PBRMaterial::SetMetallic(const float& metallic)
	{
		Metallic = std::clamp(metallic, 0.001f, 1.0f);
	}

	void PBRMaterial::SetRoughness(const float& roughness)
	{
		Roughness = std::clamp(roughness, 0.001f, 1.0f);
	}

	void PBRMaterial::SetAmbientOcclusion(const float& ambientOcclusion)
	{
		AmbientOcclusion = std::clamp(ambientOcclusion, 0.0f, 1.0f);
	}

	void PBRMaterial::SetAlpha(const float& alpha)
	{
		Alpha = std::clamp(alpha, 0.0f, 1.0f);
	}

	void PBRMaterial::OnDraw()
	{
		if (_shader.expired()) return;
		const std::shared_ptr<Shader> shader = _shader.lock();

	

		// load albedo (map_kd):
		if (!_diffuseTexture.expired() && !ManualMode)
		{
			_subroutineIndices[_albedoSubroutineUniform] = _autoAlbedo;
			shader->SetTexture("u_AlbedoMap", _diffuseTexture.lock()->_id, 1); // texture unit slots start at 1.
		}
		else
		{
			_subroutineIndices[_albedoSubroutineUniform] = _manualAlbedo;
			shader->SetVector3("u_Albedo", Albedo);
		}

		// load roughness/metallic (map_ks):
		if (!_specularTexture.expired() && !ManualMode)
		{
			_subroutineIndices[_roughnessSubroutineUniform] = _autoRoughness;
			shader->SetTexture("u_RoughnessMap", _specularTexture.lock()->_id, 2);
		}
		else
		{
			_subroutineIndices[_roughnessSubroutineUniform] = _manualRoughness;
			shader->SetFloat("u_Roughness", Roughness);
		}

		// load normals (map_Bump):
		if (!_normalTexture.expired() && !ManualMode)
		{
			_subroutineIndices[_normalSubroutineUniform] = _autoNormal;
			shader->SetTexture("u_NormalMap", _normalTexture.lock()->_id, 3);
		}
		else _subroutineIndices[_normalSubroutineUniform] = _manualNormal;

		glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, _subroutineIndices.size(), _subroutineIndices.data());

		shader->SetInt("u_IrradianceMap", 4);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_CUBE_MAP, MimicCore::EnvironmentCubeMap->_irradianceMapTextureId);

		shader->SetInt("u_PrefilterMap", 5);
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_CUBE_MAP, MimicCore::EnvironmentCubeMap->_prefilteredMapTextureId);

		shader->SetInt("u_BRDFLookupTexture", 6);
		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, MimicCore::EnvironmentCubeMap->_brdfConvolutedTextureId);
		// not sure assimp ever loads this:
		/*if (!_heightTexture.expired()) shader->SetTexture("u_Height", _heightTexture.lock()->_id, 4);*/

		shader->SetVector3("u_Emissive", Emissive);
		shader->SetFloat("u_Alpha", Alpha);
		shader->SetFloat("u_AmbientOcclusion", AmbientOcclusion);
		// shader->SetMat3("u_NormalMatrix", glm::transpose(glm::inverse(glm::mat3(_gameObject.lock()->_modelMatrix))));
	}
}