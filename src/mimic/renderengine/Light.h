#pragma once
#include <GLM/glm.hpp>
#include <string>

namespace Mimic
{
	// Source: https://learnopengl.com/Lighting/Light-casters
	// #############################################################################
	// direct light class:
	// #############################################################################
	struct DirectLight
	{
		explicit DirectLight();
		explicit DirectLight(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& colour);
		
		glm::vec3 Position;
		glm::vec3 Direction;
		
		glm::vec3 Colour;
		std::string Name;
	private:
		// glm::mat4 _modelMatrix;
	};
}