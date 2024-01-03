#pragma once
#include "Rigidbody.h"

#include <vector>
#include <memory>

namespace MimicPhysics
{
	// #############################################################################
	// Simulation Struct:
	// ##############################################################################
	// Source: https://books.google.co.uk/books/about/Physics_for_Game_Developers.html?id=YHamXqda_swC&redir_esc=y
	static void StepSimulation(const float& deltaTime, std::vector<std::shared_ptr<Rigidbody>>& rigidbodies)
	{
		// Calculate all forces and movements on all objects:
		// CalcObjectForces();

		for (auto rigidbody : rigidbodies)
		{
			// Calculate acceleration:
			const glm::vec3 Ae = rigidbody->GetForces() / rigidbody->GetMass();
			rigidbody->SetAcceleration(Ae);

			// Calculate velocity:
			rigidbody->SetVelocity(Ae * deltaTime);

			// Calculate position:
			rigidbody->SetPosition(rigidbody->GetPosition() + (rigidbody->GetVelocity() * deltaTime));
		}
	}
}