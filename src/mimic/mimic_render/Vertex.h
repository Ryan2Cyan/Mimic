#pragma once
#include <GLM/glm.hpp>
namespace MimicRender
{
	/// <summary>
	/// Stores a position, normal, tangent, bi-tangent, and texture coordinates. Generally used for rendering
	/// (see Mesh, Model, and Renderer structs).
	/// </summary>
	struct Vertex
	{
		static Vertex Initialise(const glm::vec3& position, const glm::vec3& normal, const glm::vec3& tangent, const glm::vec3& biTangent, const glm::vec2& textureCoordinates);
		glm::vec3 Position;

	private:
		friend struct Mesh;
		friend struct Model;

		glm::vec3 _normal;
		glm::vec3 _tangent;
		glm::vec3 _biTangent;
		glm::vec2 _textureCoordinates;
	};
}