#pragma once
#include <GLM/glm.hpp>

namespace Mimic
{
	// #############################################################################
	// vertex stuct:
	// #############################################################################
	struct Vertex
	{
		explicit Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 textureCoordinates);

		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TextureCoordinates;
	};
}