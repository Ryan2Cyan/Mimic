#include "Material.h"
#include <lowlevelsystems/MimicCore.h>
#include <lowlevelsystems/ResourceManager.h>
#include <renderengine/Shader.h>
#include <renderengine/Texture.h>

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
		if (!_diffuseTexture.expired()) shader->SetTexture("diffuse", _diffuseTexture.lock()->_id, 0);
		if (!_specularTexture.expired()) shader->SetTexture("specular", _specularTexture.lock()->_id, 1);
		if (!_normalTexture.expired()) shader->SetTexture("normal", _normalTexture.lock()->_id, 2);
		if (!_heightTexture.expired()) shader->SetTexture("height", _heightTexture.lock()->_id, 3);
	}
}