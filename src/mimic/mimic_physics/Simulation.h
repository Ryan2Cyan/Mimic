#pragma once
#include "Rigidbody.h"

#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <memory>

namespace MimicPhysics
{
	// #############################################################################
	// Simulation Struct:
	// ##############################################################################
	static void StepSimulation(const float& deltaTime, std::vector<std::shared_ptr<Rigidbody>>& rigidbodies)
	{
		// Source: https://books.google.co.uk/books/about/Physics_for_Game_Developers.html?id=YHamXqda_swC&redir_esc=y
		// Calculate all forces and movements on all objects:
		// CalcObjectForces();

		for (auto rigidbody : rigidbodies)
		{
			// Calculate acceleration (earth space):
			const glm::vec3 Ae = rigidbody->GetForces() / rigidbody->GetMass();
			rigidbody->SetAcceleration(Ae);

			// Calculate velocity (earth space):
			rigidbody->SetVelocity(Ae * deltaTime);

			// Calculate position (earth space):
			rigidbody->SetPosition(rigidbody->GetPosition() + (rigidbody->GetVelocity() * deltaTime));

			// Handle rotations:
			float mag = {};
			rigidbody->SetAngularAcceleration(rigidbody->GetInertiaInverse() * (rigidbody->GetMovement() - (rigidbody->GetAngularVelocity() ^ (rigidbody->GetAngularVelocity()))));
			rigidbody->SetAngularVelocity(rigidbody->GetAngularAcceleration() * deltaTime);

			// Calculate new rotation quaternion, and normalise:
			// rigidbody->SetQuatOrientation(rigidbody->GetQuatOrientation() + ((rigidbody->GetQuatOrientation() * rigidbody->GetAngularVelocity()) * (0.5f * deltaTime)));

			// Calculate velocity (body space):
			/*glm::vec4 vec4Quat = glm::vec4(rigidbody->GetQuatOrientation(), 0.0f);
			auto rotMat = glm::toMat4(vec4Quat);*/
			// rigidbody->SetVelocityBody(glm::rotate(glm::vec4(~rigidbody->GetVelocity(), 0.0f)), glm::mat4_cast(rigidbody->GetQuatOrientation()));


		}
	}

	
}