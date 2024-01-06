#include "Triangle.h"

namespace MimicRender
{
	Triangle::Triangle() : v0(glm::vec3(0.0f)), v1(glm::vec3(0.0f)), v2(glm::vec3(0.0f)) {}
	Triangle::Triangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2) : v0(v0), v1(v1), v2(v2) {}
}