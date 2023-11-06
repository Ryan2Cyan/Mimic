#include "Vertex.h"

namespace Mimic
{
	Vertex::Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec3& tangent, const glm::vec3& biTangent, const glm::vec2& textureCoordinates) 
		: _position(position), _normal(normal), _tangent(tangent), _biTangent(biTangent), _textureCoordinates(textureCoordinates){ }
}