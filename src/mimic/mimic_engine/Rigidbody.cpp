#include "Rigidbody.h"
#include "MimicCore.h"
#include "Collider.h"

namespace MimicEngine
{
	void Rigidbody::Initialise()
	{

	}

	void Rigidbody::Start()
	{

	}

	void Rigidbody::Update()
	{
	
	}

	void Rigidbody::FixedUpdate()
	{
		// Check if the current GameObject has a collider.
		const auto gameObject = GetGameObject();
		const auto boxCollider = gameObject->GetComponent<BoxCollider>();
		const auto meshCollider = gameObject->GetComponent<MeshCollider>();
		if (!boxCollider && !meshCollider) return;

		const auto mimicCore = gameObject->GetMimicCore();
		const auto allBoxColliders = mimicCore->Find<BoxCollider>();
		const auto allMeshColliders = mimicCore->Find<MeshCollider>();

		// Fetch all colliders in the scene.
		// If GameObject has a BoxCollider, check collisions.
		if (boxCollider)
		{
			for (const auto boxIter : allBoxColliders)
			{
				if (boxCollider == boxIter) continue;
				if (boxCollider->IsColliding(boxIter))
				{
					// Call on-collision:
					MIMIC_DEBUG_LOG("box-box collision");
					ResolveCollision(boxCollider->GetCollisionResponse(boxIter));
				}
			}

			for (const auto meshIter : allMeshColliders)
			{
				if (boxCollider->IsColliding(meshIter))
				{
					// Call on-collision:
					MIMIC_DEBUG_LOG("box-mesh collision");
					ResolveCollision(boxCollider->GetCollisionResponse(meshIter));
				}
			}
		}

		// If GameObject has a MeshCollider, check collisions.
		if (meshCollider)
		{
			for (const auto boxIter : allBoxColliders)
			{
				if (meshCollider->IsColliding(boxIter))
				{
					// Call on-collision:
					MIMIC_DEBUG_LOG("mesh-box collision");
					ResolveCollision(meshCollider->GetCollisionResponse(boxIter));
				}
			}

			for (const auto meshIter : allMeshColliders)
			{
				if (meshCollider == meshIter) continue;
				if (meshCollider->IsColliding(meshIter))
				{
					// Call on-collision:
					MIMIC_DEBUG_LOG("mesh-mesh collision");
					ResolveCollision(meshCollider->GetCollisionResponse(meshIter));
				}
			}
		}

		// Cache the position of the transform to revert back to it if
		// there is a collision:
		SetPreviousPosition(GetGameObject()->Position);
	}

	void Rigidbody::SetPreviousPosition(const glm::vec3& prevPos)
	{
		_previousPosition = prevPos;
	}

	void Rigidbody::ResolveCollision(const glm::vec3& response)
	{
		GetGameObject()->Position = response;
	}
}