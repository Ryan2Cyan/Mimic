#pragma once
#include "Component.h"
#include <mimic_physics/Collider.h>

namespace MimicEngine
{
	// #############################################################################
	// Collider Struct:
	// #############################################################################
	struct Collider
	{
		void SetOffset(const glm::vec3& offset)
		{
			_offset = offset;
		}

		glm::vec3 GetOffset() const
		{
			return _offset;
		}

	protected:
		glm::vec3 _offset = glm::vec3(0.0f);
	};

	// #############################################################################
	// BoxCollider Struct:
	// #############################################################################
	struct BoxCollider : Collider, Component
	{
		void Start() override;
		void Update() override;

		void SetSize(const glm::vec3& size);
		glm::vec3 GetSize() const;
		bool IsInitialised() const;

		/// <summary> Returns true if colliding with another mesh collider, otherwise false </summary>
		bool IsColliding(const std::shared_ptr<BoxCollider> collider, const bool& aligned = false);
		bool IsColliding(const std::shared_ptr<MeshCollider> collider);

		/*/// <summary> Returns a translation vector 3 in response to a collision, to resolve
		/// (not colliding anymore) the collision. </summary>
		glm::vec3 GetCollisionResponse(const glm::vec3& position, const glm::vec3 size);*/

	private:
		friend struct MeshCollider;

		glm::vec3 _size = glm::vec3(1.0f);
		std::shared_ptr<MimicPhysics::BoxCollider> _physicsBoxCollider;
	};

	// #############################################################################
	// MeshCollider Struct:
	// #############################################################################
	struct MeshCollider : Collider, Component
	{
		void Start() override;
		void Update() override;

		bool IsInitialised() const;

		/// <summary> Returns true if colliding with another mesh collider, otherwise false </summary>
		bool IsColliding(const std::shared_ptr<BoxCollider> collider);
		bool IsColliding(const std::shared_ptr<MeshCollider> collider);

		/*/// <summary> Returns a translation vector 3 in response to a collision, to resolve
		/// (not colliding anymore) the collision. </summary>
		glm::vec3 GetCollisionResponse(const glm::vec3& position, const glm::vec3 size);*/

	private:
		friend struct BoxCollider;
		std::shared_ptr<MimicPhysics::MeshCollider> _physicsMeshCollider;
	};
}