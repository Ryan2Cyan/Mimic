#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace MimicPhysics
{
	// #############################################################################
	// Collider struct:
	// #############################################################################
	struct Collider
	{
		void SetOffset(const glm::vec3& offset);
		glm::vec3 GetOffset() const;
		bool IsInitialised() const;

	protected:
		glm::vec3 _offset;
		bool _initialised;
	};

	// #############################################################################
	// MeshCollider struct:
	// #############################################################################
	struct MeshCollider : Collider
	{
		static std::shared_ptr<MeshCollider> Initialise(const std::vector<glm::vec3>& vertices);

		/// <summary> Returns true if colliding with another mesh collider, otherwise 
		/// false </summary>
		bool IsColliding(const std::shared_ptr<MeshCollider>& collider) const;

		std::vector<glm::vec3> GetVertices() const;

		/// <summary> Returns the furthest vertex in any given direction. </summary>
		glm::vec3 GetFarthestPoint(const glm::vec3& direction) const;

 	private:
		std::vector<glm::vec3> _vertices;
		std::weak_ptr<MeshCollider> _self;
	};
}