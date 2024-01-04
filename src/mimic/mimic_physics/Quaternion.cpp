#include "Quaternion.h"

namespace MimicPhysics
{
	Quaternion::Quaternion() : _numberScalar(0.0f), _vector(glm::vec3(0.0f)) {}

	Quaternion::Quaternion(const float& e0, const glm::vec3& vector) : _numberScalar(e0), _vector(vector) {}

	Quaternion::Quaternion(const float& e0, const float& e1, const float& e2, const float& e3) : _numberScalar(e0), _vector(glm::vec3(e1, e2, e3)) {}

	float Quaternion::Magnitude() const
	{
		return glm::sqrt(_numberScalar * _numberScalar + _vector.x * _vector.x + _vector.y * _vector.y + _vector.z * _vector.z);
	}

	glm::vec3 Quaternion::GetVector() const
	{
		return _vector;
	}

	float Quaternion::GetScalar() const
	{
		return _numberScalar;
	}
}