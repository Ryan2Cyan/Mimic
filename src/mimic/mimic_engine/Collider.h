#pragma once
#include "Component.h"

namespace MimicPhysics
{
	struct BoxCollider;
	struct MeshCollider;
}

#include <functional>
namespace MimicEngine
{
	/// <summary>
	/// Collider: Base class for collider components containing both the collider's offset (from it's parent GameObject), and 
	/// a bool relaying whether a collision has occured or not.
	/// </summary>
	struct Collider
	{
		void SetOffset(const glm::vec3& offset);
		void SetIsColliding(const bool& colliding);

		glm::vec3 GetOffset() const;
		bool IsColliding() const;

	protected:
		glm::vec3 _offset = glm::vec3(0.0f);
		bool _colliding;
	};

	/// <summary>
	/// BoxCollider: Wrapper for MimicPhysics::BoxCollider. Collision entity shaped like a cuboid. User can modify its size on the x, y, and z axes.
	/// User can set the 'OnCollisionEnter' and 'OnCollisionExit' functions to execute code.
	/// </summary>
	struct BoxCollider : Collider, Component
	{
		void Initialise() override;
		void Start() override;
		void Update() override;
		void FixedUpdate() override;

		void SetSize(const glm::vec3& size);
		glm::vec3 GetSize() const;
		bool IsInitialised() const;

		/// <summary> Called when a collision occurs. </summary>
		std::function<void()> OnCollisionEnter;

		/// <summary> Called when a collision has ceased. </summary>
		std::function<void()> OnCollisionExit;

	private:
		friend struct MeshCollider;
		friend struct Rigidbody;

		bool IsColliding(const std::shared_ptr<BoxCollider> collider, const bool& aligned = false);
		bool IsColliding(const std::shared_ptr<MeshCollider> collider);
		glm::vec3 GetCollisionResponse(const std::shared_ptr<BoxCollider> collider);
		glm::vec3 GetCollisionResponse(const std::shared_ptr<MeshCollider> collider);

		glm::vec3 _size = glm::vec3(1.0f);
		std::shared_ptr<MimicPhysics::BoxCollider> _physicsBoxCollider;
	};

	/// <summary>
	/// MeshCollider: Wrapper for MimicPhysics::MeshCollider. Will check if the user has a ModelRenderer and
	/// valid Model/Meshes to extract vertices, if one of these conditions is not met the MeshCollider will fail
	/// to initialise correctly. User can modify its size on the x, y, and z axes. User can set the 'OnCollisionEnter' 
	/// and 'OnCollisionExit' functions to execute code.
	/// </summary>
	struct MeshCollider : Collider, Component
	{
		void Initialise() override;
		void Start() override;
		void Update() override;
		void FixedUpdate() override;

		bool IsInitialised() const;

		/// <summary> Called when a collision occurs. </summary>
		std::function<void()> OnCollisionEnter;

		/// <summary> Called when a collision has ceased. </summary>
		std::function<void()> OnCollisionExit;

	private:
		friend struct BoxCollider;
		friend struct Rigidbody;

		bool IsColliding(const std::shared_ptr<BoxCollider> collider);
		bool IsColliding(const std::shared_ptr<MeshCollider> collider);
		glm::vec3 GetCollisionResponse(const std::shared_ptr<BoxCollider> collider);
		glm::vec3 GetCollisionResponse(const std::shared_ptr<MeshCollider> collider);

		std::shared_ptr<MimicPhysics::MeshCollider> _physicsMeshCollider;
	};
}