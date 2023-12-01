#include "Light.h"
#include <renderengine/RenderTexture.h>

namespace MimicRender
{
	// #############################################################################
	// direct light functions:
	// #############################################################################
	const std::shared_ptr<DirectLight> DirectLight::Initialise() noexcept
	{
		return std::make_shared<DirectLight>();
	}

	const std::shared_ptr<DirectLight> DirectLight::Initialise(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& colour) noexcept
	{
		std::shared_ptr<DirectLight> directLight = std::make_shared<DirectLight>();
		directLight->Position = position;
		directLight->Direction = direction;
		directLight->Colour = colour;
		return directLight;
	}

	const unsigned int DirectLight::GetDepthMapTextureId() const noexcept
	{
		return _depthMapRT->GetTextureID();
	}

	const glm::mat4 DirectLight::GetLightMatrix() const noexcept
	{
		return _lightMatrix;
	}


	// #############################################################################
	// point light functions:
	// #############################################################################
	const std::shared_ptr<PointLight> PointLight::Initialise() noexcept
	{
		return std::make_shared<PointLight>();
	}

	const std::shared_ptr<PointLight> PointLight::Initialise(const glm::vec3& position, const glm::vec3& colour) noexcept
	{
		std::shared_ptr<PointLight> pointLight = std::make_shared<PointLight>();
		pointLight->Position = position;
		pointLight->Colour = colour;
		return pointLight;
	}

	const float PointLight::GetConstant() const noexcept
	{
		return _constant;
	}

	const float PointLight::GetLinear() const noexcept
	{
		return _linear;
	}

	const float PointLight::GetQuadratic() const noexcept
	{
		return _quadratic;
	}
}