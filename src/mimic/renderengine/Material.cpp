#include "Material.h"
#include <lowlevelsystems/MimicCore.h>
#include <lowlevelsystems/ResourceManager.h>
#include <renderengine/Shader.h>
#include <renderengine/Texture.h>
#include <algorithm>

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
		_shader = MimicCore::ResourceManager->LoadResource<Shader>("PBRShader.glsl");
		SetAlbedo(glm::vec3(0.0f));
		SetEmissive(glm::vec3(0.0f));
		SetMetallic(0.0f);
		SetRoughness(0.0f);
		SetAmbientOcclusion(0.0f);
		SetAlpha(1.0f);
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

	void PBRMaterial::OnDraw()
	{
		if (_shader.expired()) return;
		const std::shared_ptr<Shader> shader = _shader.lock();
		if (!_diffuseTexture.expired()) shader->SetTexture("u_Diffuse", _diffuseTexture.lock()->_id, 0);
		if (!_specularTexture.expired()) shader->SetTexture("u_Specular", _specularTexture.lock()->_id, 1);
		if (!_normalTexture.expired()) shader->SetTexture("u_Normal", _normalTexture.lock()->_id, 2);
		if (!_heightTexture.expired()) shader->SetTexture("u_Height", _heightTexture.lock()->_id, 3);

		shader->SetVector3("u_Albedo", _albedo);
		shader->SetVector3("u_Emissive", _emissive);
		shader->SetFloat("u_Metallic", _metallic);
		shader->SetFloat("u_Roughness", _roughness);
		shader->SetFloat("u_Alpha", _alpha);
		shader->SetFloat("u_AmbientOcclusion", _ambientOcclusion);
	}
}