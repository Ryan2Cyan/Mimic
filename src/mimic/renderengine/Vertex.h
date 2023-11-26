#pragma once
#include <GLM/glm.hpp>
namespace MimicRender
{
	// #############################################################################
	// vertex stuct:
	// #############################################################################
	struct Vertex
	{
		static const Vertex Initialise(const glm::vec3& position, const glm::vec3& normal, const glm::vec3& tangent, const glm::vec3& biTangent, const glm::vec2& textureCoordinates);
	private:
		friend struct Mesh;
		friend struct Model;

		glm::vec3 _position;
		glm::vec3 _normal;
		glm::vec3 _tangent;
		glm::vec3 _biTangent;
		glm::vec2 _textureCoordinates;
	};
}