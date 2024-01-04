#pragma once
#include "Rigidbody.h"
#include "Collision.h"

#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <memory>

#define RHO           1.225f

namespace MimicPhysics
{
	// #############################################################################
	// Simulation:
	// ##############################################################################
	static float fixedDeltaTime = 0.2f;

	static void ResolveCollisions(std::vector<std::shared_ptr<Collider>>& rigidbodies)
	{

	}
	//static glm::vec3 Thrust;
	//static float ThrustForce;

	///// <summary> Handle all forces (e.g. thrust, aerodynamic drag, gravity, and ground plane contact)
	///// for all objects in the simulations. </summary>
	//static void CalculateObjectForces(std::vector<std::shared_ptr<Rigidbody>>& rigidbodies)
	//{
	//	auto index = 0;
	//	auto forces = glm::vec3(0.0f);
	//	auto movement = glm::vec3(0.0f);
	//	auto dragForce = glm::vec3(0.0f);
	//	auto angularDrag = glm::vec3(0.0f);

	//	for (auto rigidbody : rigidbodies)
	//	{
	//		// Reset forces and movement:
	//		rigidbody->SetForces(glm::vec3(0.0f));
	//		rigidbody->SetMovement(glm::vec3(0.0f));
	//		forces = glm::vec3(0.0f);
	//		movement = glm::vec3(0.0f);

	//		// Define thrust vector:
	//		if (index == 0)
	//		{
	//			Thrust = glm::vec3(1.0f, 0.0f, 0.0f) * ThrustForce; 
	//			forces += Thrust;
	//		}

	//		// Drag force:
	//		dragForce = glm::normalize(rigidbody->GetVelocityBody());
	//		forces += dragForce * (rigidbody->GetSpeed() * rigidbody->GetSpeed() * RHO * )
	//	}
	//}

	//static void StepSimulation(const float& deltaTime, std::vector<std::shared_ptr<Rigidbody>>& rigidbodies)
	//{
	//	// Source: https://books.google.co.uk/books/about/Physics_for_Game_Developers.html?id=YHamXqda_swC&redir_esc=y
	//	// Calculate all forces and movements on all objects:
	//	// CalcObjectForces();

	//	for (auto rigidbody : rigidbodies)
	//	{
	//		// Calculate acceleration (earth space):
	//		const glm::vec3 Ae = rigidbody->GetForces() / rigidbody->GetMass();
	//		rigidbody->SetAcceleration(Ae);

	//		// Calculate velocity (earth space):
	//		rigidbody->SetVelocity(Ae * deltaTime);

	//		// Calculate position (earth space):
	//		rigidbody->SetPosition(rigidbody->GetPosition() + (rigidbody->GetVelocity() * deltaTime));

	//		// Handle rotations:
	//		float mag = {};
	//		rigidbody->SetAngularAcceleration(rigidbody->GetInertiaInverse() * (rigidbody->GetMovement() - (rigidbody->GetAngularVelocity() ^ (rigidbody->GetAngularVelocity()))));
	//		rigidbody->SetAngularVelocity(rigidbody->GetAngularAcceleration() * deltaTime);

	//		// Calculate new rotation quaternion, and normalise:
	//		rigidbody->SetQuatOrientation(rigidbody->GetQuatOrientation() + ((rigidbody->GetQuatOrientation() * rigidbody->GetAngularVelocity()) * (0.5f * deltaTime)));
	//		const auto magnitude = rigidbody->GetQuatOrientation().Magnitude();
	//		if (magnitude > 0.0f || magnitude < 0.0f) rigidbody->SetQuatOrientation(rigidbody->GetQuatOrientation() / magnitude);
	//		
	//		// Calculate velocity (body space):
	//		rigidbody->SetVelocityBody(QuatVec3Rotate(rigidbody->GetVelocity(), ~rigidbody->GetQuatOrientation()));

	//		// Calculate speed:
	//		rigidbody->SetSpeed(rigidbody->GetVelocity().length());

	//		// Get eular angles:
	//		rigidbody->SetEularAngles(EularAnglesFromQuat(rigidbody->GetQuatOrientation());
	//	}

	//	// Handle collisions:
	//}

	//static void ResolveCollisions()
	//{

	//}
}