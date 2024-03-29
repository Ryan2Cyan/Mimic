#include "Vertex.h"

namespace MimicRender
{
	Vertex Vertex::Initialise(const glm::vec3& position, const glm::vec3& normal, const glm::vec3& tangent, const glm::vec3& biTangent, const glm::vec2& textureCoordinates)
	{
		Vertex vertex;
		vertex.Position = position;
		vertex._normal = normal;
		vertex._tangent = tangent;
		vertex._biTangent = biTangent;
		vertex._textureCoordinates = textureCoordinates;
		return vertex;
	}
}