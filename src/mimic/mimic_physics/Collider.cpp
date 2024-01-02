#include "Collider.h"
#include "Collision.h"

namespace MimicPhysics
{
	// #############################################################################
	// Collider functions:
	// #############################################################################
	void Collider::SetOffset(const glm::vec3& offset)
	{
		_offset = offset;
	}

	glm::vec3 Collider::GetOffset() const
	{
		return _offset;
	}

	bool Collider::IsInitialised() const
	{
		return _initialised;
	}

	// #############################################################################
	// MeshCollider functions:
	// #############################################################################
	std::shared_ptr<MeshCollider> MeshCollider::Initialise(const std::vector<glm::vec3>& vertices)
	{
		if (vertices.size() <= 0) return nullptr;

		auto meshCollider = std::make_shared<MeshCollider>();
		meshCollider->_vertices = vertices;
		meshCollider->_initialised = true;
		meshCollider->_self = meshCollider;
		return meshCollider;
	}

	bool MeshCollider::IsColliding(const std::shared_ptr<MeshCollider>& collider) const
	{
		return GJKCollisionDetection(_self.lock(), collider);
	}

	std::vector<glm::vec3> MeshCollider::GetVertices() const
	{
		return _vertices;
	}


	glm::vec3 MeshCollider::GetFarthestPoint(const glm::vec3& direction) const
	{
		// Source: https://en.wikipedia.org/wiki/Minkowski_addition

		auto farthestPoint = glm::vec3(0);
		float max = std::numeric_limits<float>::min();

		// Loop through all vertices, check the dot product with the input direction,
		// and cache result if larger than the last:
		for (auto vertex : _vertices)
		{
			const float projection = glm::dot(vertex, direction);
			if (projection > max)
			{
				farthestPoint = vertex;
				max = projection;
			}
		}
		return farthestPoint + _offset;
	}
}