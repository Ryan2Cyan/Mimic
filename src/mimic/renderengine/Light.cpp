#include "Light.h"

namespace Mimic
{
	// #############################################################################
	// direct light class:
	// #############################################################################
	DirectLight::DirectLight() : Position(glm::vec3(0.0f)), Direction(glm::vec3(0.0f)), Colour(glm::vec3(0.0f)){}
	DirectLight::DirectLight(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& colour) 
		: Position(glm::vec3(position)), Direction(glm::vec3(direction)), Colour(glm::vec3(colour)){}
}