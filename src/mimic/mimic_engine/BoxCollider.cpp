#include "BoxCollider.h"
#include <mimic_utility/Logger.h>

namespace MimicEngine
{

	void BoxCollider::Start()
	{

	}

	void BoxCollider::Update()
	{

	}

	bool BoxCollider::IsColliding(const glm::vec3& position, const glm::vec3 size)
	{
		return false;
	}

	glm::vec3 BoxCollider::GetCollisionResponse(const glm::vec3& position, const glm::vec3 size)
	{
		return glm::vec3(0.0f);
	}

	void BoxCollider::SetSize(const glm::vec3& size)
	{
		_size = size;
	}

	void BoxCollider::SetOffset(const glm::vec3& offset)
	{
		_offset = offset;
	}

	glm::vec3 BoxCollider::GetFarthestPoint(const glm::vec3 direction) const
	{
		// Add position onto each point to get it's world pos:
		auto position = GetGameObject()->Position + _offset;
		auto halfSize = _size * 0.5f;
		auto vertices =
		{
			position + glm::vec3(-halfSize.x, halfSize.y, -halfSize.z),
			position + glm::vec3(-halfSize.x, -halfSize.y, -halfSize.z),
			position + glm::vec3(halfSize.x, halfSize.y, -halfSize.z),
			position + glm::vec3(halfSize.x, -halfSize.y, -halfSize.z),

			position + glm::vec3(-halfSize.x, halfSize.y, halfSize.z),
			position + glm::vec3(-halfSize.x, -halfSize.y, halfSize.z),
			position + glm::vec3(halfSize.x, halfSize.y, halfSize.z),
			position + glm::vec3(halfSize.x, -halfSize.y, halfSize.z)
		};

		auto farthestPoint = glm::vec3(0);
		float max = 0.0f;

		// Loop through all vertices, check the dot product with the direction,
		// and cache result if larger than the last, whilst caching the furthest
		// point:
		for (auto vertex : vertices)
		{
			const float projection = glm::dot(direction, vertex);
			if (projection > max)
			{
				farthestPoint = vertex;
				max = projection;
			}
		}
		MIMIC_DEBUG_LOG("Result: (%, %, %)", farthestPoint.x, farthestPoint.y, farthestPoint.z);
		return farthestPoint;
	}
}