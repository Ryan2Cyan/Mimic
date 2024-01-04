#include "Rigidbody.h"

namespace MimicPhysics
{
	// #############################################################################
	// Rigidbody Functions:
	// ##############################################################################
	std::shared_ptr<Rigidbody> Rigidbody::Initialise()
	{
		return std::make_shared<Rigidbody>();
	}

	void Rigidbody::SetPosition(const glm::vec3& position)
	{
		_position = position;
	}

	void Rigidbody::SetVelocity(const glm::vec3& velocity)
	{
		_velocity = velocity;
	}

	void Rigidbody::SetAcceleration(const glm::vec3& acceleration)
	{
		_acceleration = acceleration;
	}

	void Rigidbody::SetForces(const glm::vec3& forces)
	{
		_forces = forces;
	}
	void Rigidbody::SetMovement(const glm::vec3& movement)
	{
		_movement = movement;
	}

	void Rigidbody::SetAngularVelocity(const glm::vec3& angVelocity)
	{
		_angularVelocity = angVelocity;
	}

	void Rigidbody::SetAngularAcceleration(const glm::vec3& angAcceleration)
	{
		_angularAcceleration = angAcceleration;
	}

	void Rigidbody::SetVelocityBody(const glm::vec3& velocityBody)
	{
		_velocityBody = velocityBody;
	}

	void Rigidbody::SetQuatOrientation(const Quaternion& orientation)
	{
		_quatOrientation = orientation;
	}

	void Rigidbody::SetOrientation(const float& orientation)
	{
		_orientation = orientation;
	}

	void Rigidbody::SetMass(const float& mass)
	{
		_mass = mass;
	}

	void Rigidbody::SetInertia(const float& inertia)
	{
		_inertia = inertia;
	}

	void Rigidbody::SetInertiaInverse(const float& inertiaInv)
	{
		_inertiaInverse = inertiaInv;
	}

	glm::vec3 Rigidbody::GetPosition() const
	{
		return _position;
	}

	glm::vec3 Rigidbody::GetVelocity() const
	{
		return _velocity;
	}

	glm::vec3 Rigidbody::GetAcceleration() const
	{
		return _acceleration;
	}

	glm::vec3 Rigidbody::GetForces() const
	{
		return _forces;
	}

	glm::vec3 Rigidbody::GetMovement() const
	{
		return _movement;
	}

	glm::vec3 Rigidbody::GetAngularVelocity() const
	{
		return _angularVelocity;
	}

	glm::vec3 Rigidbody::GetAngularAcceleration() const
	{
		return _angularAcceleration;
	}

	glm::vec3 Rigidbody::GetVelocityBody() const
	{
		return _velocityBody;
	}

	Quaternion Rigidbody::GetQuatOrientation() const
	{
		return _quatOrientation;
	}

	float Rigidbody::GetOrientation() const
	{
		return _orientation;
	}

	float Rigidbody::GetMass() const
	{
		return _mass;
	}

	float Rigidbody::GetInertia() const
	{
		return _inertia;
	}

	float Rigidbody::GetInertiaInverse() const
	{
		return _inertiaInverse;
	}

	bool Rigidbody::IsInitialised() const
	{
		return _initialised;
	}

	void Rigidbody::CalcLoads(std::shared_ptr<Rigidbody>& body)
	{
		glm::vec3 forceSum = glm::vec3(0.0f);
		glm::vec3 movementSum = glm::vec3(0.0f);
		glm::vec3 thrust = glm::vec3(1.0f, 0.0f, 0.0f);


		// Reset forces and movements:
		body->SetForces(glm::vec3(0.0f));
		body->SetMovement(glm::vec3(0.0f));
	}

	void Rigidbody::UpdateBody()
	{

	}
}