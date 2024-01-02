#pragma once
#include "Component.h"
#include <mimic_physics/Collider.h>

namespace MimicEngine
{
	// #############################################################################
	// MeshCollider Struct:
	// #############################################################################
	struct MeshCollider : Component
	{
		void Start() override;
		void Update() override;

		/// <summary> Returns true if colliding with another mesh collider, otherwise 
		/// false </summary>
		bool IsColliding(const std::shared_ptr<MeshCollider> collider);

		/*/// <summary> Returns a translation vector 3 in response to a collision, to resolve
		/// (not colliding anymore) the collision. </summary>
		glm::vec3 GetCollisionResponse(const glm::vec3& position, const glm::vec3 size);*/

	private:
		std::shared_ptr<MimicPhysics::MeshCollider> _physicsMeshCollider;
	};
}