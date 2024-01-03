#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <mimic_utility/Logger.h>

namespace MimicPhysics
{
	// #############################################################################
	// Collider struct:
	// #############################################################################
	struct Collider
	{
		virtual ~Collider() = default;

		void SetPosition(const glm::vec3& position);
		void SetOffset(const glm::vec3& offset);

		std::vector<glm::vec3> GetVertices() const;
		glm::vec3 GetPosition() const;
		glm::vec3 GetOffset() const;
		bool IsInitialised() const;

	protected:
		std::vector<glm::vec3> _vertices;
		glm::vec3 _offset;
		glm::vec3 _position;
		bool _initialised;
	};

	// #############################################################################
	// BoxCollider struct:
	// #############################################################################
	struct BoxCollider : Collider 
	{
		static std::shared_ptr<BoxCollider> Initialise(const glm::vec3& size, const glm::vec3& offset);

		/// <summary> Returns true if colliding with another mesh collider, otherwise 
		/// false </summary>
		template<typename T>
		bool IsColliding(const std::shared_ptr<T>& collider) const
		{
			// Check which type of collide has been inputted, and call the appropriate collision
			// detection algorithm:
			if (auto boxCollider = std::dynamic_pointer_cast<BoxCollider>(collider))
			{

			}
			if (auto meshCollider = std::dynamic_pointer_cast<MeshCollider>(collider))
			{
				GJKCollisionDetection(_self.lock(), meshCollider);
			}
			MIMIC_LOG_WARNING("[MimicPhysics::BoxCollider] Invalid collider data type input, unable to deduce collision detection function");
			return false;
		}

		void SetSize(const glm::vec3& size);

	private:
		glm::vec3 _size;
		std::weak_ptr<BoxCollider> _self;
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

 	private:
		std::weak_ptr<MeshCollider> _self;
	};
}