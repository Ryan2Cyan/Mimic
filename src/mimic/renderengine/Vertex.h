#pragma once
#include <GLM/glm.hpp>

namespace Mimic
{
	// #############################################################################
	// vertex stuct:
	// #############################################################################
	struct Vertex
	{
	private:
		friend struct Mesh;
		friend struct Model;

		explicit Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 textureCoordinates);

		glm::vec3 _position;
		glm::vec3 _normal;
		glm::vec2 _textureCoordinates;
	};
}