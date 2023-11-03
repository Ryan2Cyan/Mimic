#include "Vertex.h"

namespace Mimic
{
	Vertex::Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 textureCoordinates) : _position(position), _normal(normal), _textureCoordinates(textureCoordinates){ }
}