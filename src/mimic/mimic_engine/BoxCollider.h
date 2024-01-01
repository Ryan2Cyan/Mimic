#pragma once
#include "Component.h"
#include <mimic_render/Vertex.h>

#include <GLM/glm.hpp>
#include <vector>

namespace MimicEngine
{
	// #############################################################################
	// Box Collider Struct:
	// #############################################################################
	struct MimicRender::Vertex;

	struct MeshCollider : Component
	{
		void Start() override;
		void Update() override;

		/// <summary> Return true if colliding with another collider, false otherwise. </summary>
		bool IsColliding(const glm::vec3& position, const glm::vec3 size);

		/// <summary> Returns a translation vector 3 in response to a collision, to resolve
		/// (not colliding anymore) the collision. </summary>
		glm::vec3 GetCollisionResponse(const glm::vec3& position, const glm::vec3 size);

		glm::vec3 GetFarthestPoint(const glm::vec3 direction) const;

	private:
		std::vector<glm::vec3> _vertices;
	};
}