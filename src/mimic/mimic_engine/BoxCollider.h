#pragma once
#include "Component.h"

#include <GLM/glm.hpp>

namespace MimicEngine
{
	// #############################################################################
	// Box Collider Struct:
	// #############################################################################
	struct BoxCollider : Component
	{
		void Start() override;
		void Update() override;

		/// <summary> Return true if colliding with another collider, false otherwise. </summary>
		bool IsColliding(const glm::vec3& position, const glm::vec3 size);

		/// <summary> Returns a translation vector 3 in response to a collision, to resolve
		/// (not colliding anymore) the collision. </summary>
		glm::vec3 GetCollisionResponse(const glm::vec3& position, const glm::vec3 size);

		/// <summary> Box collider setter.</summary>
		void SetSize(const glm::vec3& size);
		void SetOffset(const glm::vec3& offset);

		glm::vec3 GetFarthestPoint(const glm::vec3 direction) const;

	private:

		glm::vec3 _size;
		glm::vec3 _offset;
	};
}