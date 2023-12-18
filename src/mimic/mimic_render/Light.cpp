#include "Light.h"
#include <mimic_render/RenderTexture.h>

namespace MimicRender
{
	// #############################################################################
	// Direct light functions:
	// #############################################################################
	std::shared_ptr<DirectLight> DirectLight::Initialise() noexcept
	{
		return std::make_shared<DirectLight>();
	}

	std::shared_ptr<DirectLight> DirectLight::Initialise(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& colour) noexcept
	{
		std::shared_ptr<DirectLight> directLight = std::make_shared<DirectLight>();
		directLight->Position = position;
		directLight->Direction = direction;
		directLight->Colour = glm::vec3(glm::clamp(colour, 0.0f, 1.0f));
		return directLight;
	}

	unsigned int DirectLight::GetDepthMapTextureId() const noexcept
	{
		return _depthMapRT->GetTextureID();
	}

	glm::mat4 DirectLight::GetLightMatrix() const noexcept
	{
		return _lightMatrix;
	}


	// #############################################################################
	// Point light functions:
	// #############################################################################
	std::shared_ptr<PointLight> PointLight::Initialise() noexcept
	{
		return std::make_shared<PointLight>();
	}

	std::shared_ptr<PointLight> PointLight::Initialise(const glm::vec3& position, const glm::vec3& colour) noexcept
	{
		std::shared_ptr<PointLight> pointLight = std::make_shared<PointLight>();
		pointLight->Position = position;
		pointLight->Colour = colour;
		return pointLight;
	}

	float PointLight::GetConstant() const noexcept
	{
		return _constant;
	}

	float PointLight::GetLinear() const noexcept
	{
		return _linear;
	}

	float PointLight::GetQuadratic() const noexcept
	{
		return _quadratic;
	}
}