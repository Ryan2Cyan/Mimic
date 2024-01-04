#pragma once
#include "Quaternion.h"

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <memory>

namespace MimicPhysics
{
	// #############################################################################
	// Rigidbody Struct:
	// ##############################################################################
	// Source: https://books.google.co.uk/books/about/Physics_for_Game_Developers.html?id=YHamXqda_swC&redir_esc=y

	struct Rigidbody
	{
		static std::shared_ptr<Rigidbody> Initialise();

		void SetPosition(const glm::vec3& position);
		void SetVelocity(const glm::vec3& velocity);
		void SetAcceleration(const glm::vec3& acceleration);
		void SetForces(const glm::vec3& forces);
		void SetMovement(const glm::vec3& movement);
		void SetAngularVelocity(const glm::vec3& angVelocity);
		void SetAngularAcceleration(const glm::vec3& angAcceleration);
		void SetVelocityBody(const glm::vec3& velocityBody);
		void SetQuatOrientation(const Quaternion& orientation);
		void SetOrientation(const float& orientation);
		void SetMass(const float& mass);
		void SetInertia(const float& inertia);
		void SetInertiaInverse(const float& inertiaInv);

		glm::vec3 GetPosition() const;
		glm::vec3 GetVelocity() const;
		glm::vec3 GetAcceleration() const;
		glm::vec3 GetForces() const;
		glm::vec3 GetMovement() const;
		glm::vec3 GetAngularVelocity() const;
		glm::vec3 GetAngularAcceleration() const;
		glm::vec3 GetVelocityBody() const;
		Quaternion GetQuatOrientation() const;
		float GetOrientation() const;
		float GetMass() const;
		float GetInertiaInverse() const;
		float GetInertia() const;
		bool IsInitialised() const;

		/// <summary> Calculates forces and movements that occurs throughout a simulation.</summary>
		static void CalcLoads(std::shared_ptr<Rigidbody>& body);

		void UpdateBody();

	private:
		glm::vec3 _position;
		glm::vec3 _velocity;
		glm::vec3 _acceleration;
		glm::vec3 _forces;
		glm::vec3 _movement;
		glm::vec3 _angularVelocity;
		glm::vec3 _angularAcceleration;
		glm::vec3 _velocityBody;
		Quaternion _quatOrientation;
		float _orientation;
		float _mass;
		float _inertia;
		float _inertiaInverse;
		bool _initialised;
	};
}