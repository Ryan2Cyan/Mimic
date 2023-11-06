#pragma once
#include <GLM/glm.hpp>

namespace Mimic
{
	// #############################################################################
	// light class:
	// #############################################################################
	struct Light
	{
		glm::vec3 Position;
		glm::vec3 Direction;
		// glm::vec3 Ambient;
		// glm::vec3 Diffuse;
		// glm::vec3 Specular;
	};
}