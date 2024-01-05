#pragma once
#include "Component.h"

struct MeshCollider;
struct BoxCollider;

namespace MimicEngine
{
	// #############################################################################
	// Rigidbody Struct:
	// #############################################################################
	struct Rigidbody : Component
	{
		void Initialise() override;
		void Start() override;
		void Update() override;
		void FixedUpdate() override;

		void SetPreviousPosition(const glm::vec3& prevPos);
		void ResolveCollision(const glm::vec3& response);

	private:
		friend struct MimicCore;

		std::vector<BoxCollider> _boxColliders;
		std::vector<MeshCollider> _meshColliders;
		glm::vec3 _previousPosition;
	};
}