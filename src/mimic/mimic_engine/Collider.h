#pragma once
#include "Component.h"
#include <mimic_physics/Collider.h>

namespace MimicEngine
{
	// #############################################################################
	// Collider Struct:
	// #############################################################################
	template<typename T = MimicPhysics::Collider>
	struct Collider : Component
	{
		void Start() override
		{
			_physicsCollider->SetPosition(GetGameObject()->Position);
		}

		void Update() override
		{
			_physicsCollider->SetPosition(GetGameObject()->Position);
			_physicsCollider->SetOffset(_offset);
		}

		void SetOffset(const glm::vec3& offset)
		{
			_offset = offset;
		}

		glm::vec3 GetOffset() const
		{
			return _offset;
		}

		std::shared_ptr<T> GetPhysicsCollider()
		{
			return _physicsCollider;
		}

		bool IsInitialised() const
		{
			return _physicsCollider->IsInitialised();
		}

		/// <summary> Returns true if colliding with another mesh collider, otherwise 
		/// false </summary>
		template<typename T> bool IsColliding(const std::shared_ptr<T> collider, const bool& aligned = false)
		{
			auto colliding = _physicsCollider->IsColliding(collider->GetPhysicsCollider(), aligned);
			if (colliding) _physicsCollider->CollisionData.push_back(CollisionData());
			return colliding;
		}

	protected:
		std::shared_ptr<T> _physicsCollider;
		glm::vec3 _offset;
	};

	// #############################################################################
	// BoxCollider Struct:
	// #############################################################################
	struct BoxCollider : Collider<MimicPhysics::BoxCollider>
	{
		void Start() override;
		void Update() override;

		void SetSize(const glm::vec3& size);
		glm::vec3 GetSize() const;

		/*/// <summary> Returns a translation vector 3 in response to a collision, to resolve
		/// (not colliding anymore) the collision. </summary>
		glm::vec3 GetCollisionResponse(const glm::vec3& position, const glm::vec3 size);*/

	private:
		glm::vec3 _size = glm::vec3(1.0f);
	};

	// #############################################################################
	// MeshCollider Struct:
	// #############################################################################
	struct MeshCollider : Collider<MimicPhysics::MeshCollider>
	{
		void Start() override;

		/*/// <summary> Returns a translation vector 3 in response to a collision, to resolve
		/// (not colliding anymore) the collision. </summary>
		glm::vec3 GetCollisionResponse(const glm::vec3& position, const glm::vec3 size);*/
	};
}