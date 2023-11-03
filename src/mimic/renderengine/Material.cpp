#include "Material.h"
#include <renderengine/Shader.h>

namespace Mimic
{
	// #############################################################################
	// basic material functions:
	// #############################################################################
	void Material::SetDiffuse(const int& diffuse)
	{
		_diffuseTexture = diffuse;
	}

	void Material::SetSpecular(const int& specular)
	{
		_specularTexture = specular;
	}

	void Material::SetNormal(const int& normal)
	{
		_normalTexture = normal;
	}

	void Material::SetHeight(const int& height)
	{
		_heightTexture = height;
	}

	// #############################################################################
	// basic material functions:
	// #############################################################################
	void BasicMaterial::OnDraw()
	{
		_shader->SetTexture("diffuse", _diffuseTexture, 0);
		_shader->SetTexture("specular", _specularTexture, 1);
		_shader->SetTexture("normal", _normalTexture, 2);
		_shader->SetTexture("height", _heightTexture, 3);
	}
}