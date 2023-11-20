#include "Light.h"

namespace Mimic
{
	// #############################################################################
	// direct light functions:
	// #############################################################################
	DirectLight::DirectLight() : Position(glm::vec3(0.0f)), Direction(glm::vec3(0.0f, 0.0f, -1.0f)), Colour(glm::vec3(0.0f))
	{

	}

	DirectLight::DirectLight(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& colour) 
		: Position(glm::vec3(position)), Direction(glm::vec3(direction)), Colour(glm::vec3(colour))
	{

	}


	// #############################################################################
	// point light functions:
	// #############################################################################
	PointLight::PointLight() : Position(glm::vec3(0.0f)), Colour(glm::vec3(0.0f)), Constant(1.0f), Linear(0.35), Quadratic(0.44)
	{

	}

	PointLight::PointLight(const glm::vec3& position, const glm::vec3& colour) : Position(position), Colour(colour), Constant(1.0f), Linear(0.35), Quadratic(0.44)
	{

	}
}