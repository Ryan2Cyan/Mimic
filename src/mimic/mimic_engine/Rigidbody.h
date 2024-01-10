#pragma once
#include "Component.h"

struct MeshCollider;
struct BoxCollider;

namespace MimicEngine
{
	/// <summary>
	/// Handles collision detection and response for accompanied collider attached to the parent GameObject.
	/// </summary>
	struct Rigidbody : Component
	{

	private:
		friend struct MimicCore;

		void Initialise() override;
		void Start() override;
		void Update() override;
		void FixedUpdate() override;

		void SetPreviousPosition(const glm::vec3& prevPos);
		void ResolveCollision(const glm::vec3& response);

		std::vector<BoxCollider> _boxColliders;
		std::vector<MeshCollider> _meshColliders;
		glm::vec3 _previousPosition;
	};
}