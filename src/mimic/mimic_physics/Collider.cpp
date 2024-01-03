#include "Collider.h"
#include "Collision.h"

namespace MimicPhysics
{
	// #############################################################################
	// Collider functions:
	// #############################################################################
	void Collider::SetPosition(const glm::vec3& position)
	{
		_position = position;
	}

	void Collider::SetOffset(const glm::vec3& offset)
	{
		_offset = offset;
	}

	std::vector<glm::vec3> Collider::GetVertices() const
	{
		return _vertices;
	}

	glm::vec3 Collider::GetPosition() const
	{
		return _position;
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
	// BoxCollider functions:
	// #############################################################################
	std::shared_ptr<BoxCollider> BoxCollider::Initialise(const glm::vec3& size, const glm::vec3& offset)
	{
		auto boxCollider = std::make_shared<BoxCollider>();
		boxCollider->_self = boxCollider;
		boxCollider->_vertices =
		{
			glm::vec3(-1.0f, 1.0f, -1.0f),
			glm::vec3(1.0f, 1.0f, -1.0f),
			glm::vec3(-1.0f, -1.0f, -1.0f),
			glm::vec3(1.0f, -1.0f, -1.0f),
			glm::vec3(-1.0f, 1.0f, 1.0f),
			glm::vec3(-1.0f, -1.0f, 1.0f),
			glm::vec3(1.0f, 1.0f, 1.0f),
			glm::vec3(1.0f, -1.0f, 1.0f)
		};
		return boxCollider;
	}

	/*bool BoxCollider::IsColliding(const std::shared_ptr<BoxCollider>& collider) const
	{
		return false;
	}*/

	void BoxCollider::SetSize(const glm::vec3& size)
	{
		_size = size;
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
}