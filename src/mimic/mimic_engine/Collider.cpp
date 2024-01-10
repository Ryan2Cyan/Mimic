#include "Collider.h"
#include "ModelRenderer.h"
#include "Model.h"
#include <mimic_render/Vertex.h>
#include <mimic_render/Mesh.h>
#include <mimic_render/Model.h>
#include <mimic_physics/Collider.h>

namespace MimicEngine
{
	// #############################################################################
	// Collider Functions:
	// #############################################################################
	void Collider::SetOffset(const glm::vec3& offset)
	{
		_offset = offset;
	}

	void Collider::SetIsColliding(const bool& colliding)
	{
		_colliding = colliding;
	}

	glm::vec3 Collider::GetOffset() const
	{
		return _offset;
	}

	bool Collider::IsColliding() const
	{
		return _colliding;
	}

	// #############################################################################
	// BoxCollider Functions:
	// #############################################################################
	void BoxCollider::Initialise()
	{
		_physicsBoxCollider = MimicPhysics::BoxCollider::Initialise(_size, _offset);
	}

	void BoxCollider::Start()
	{
		
	}

	void BoxCollider::Update()
	{

	}


	void BoxCollider::FixedUpdate()
	{
		_physicsBoxCollider->SetPosition(GetGameObject()->Position);
	}

	void BoxCollider::SetSize(const glm::vec3& size)
	{
		_size = size;
	}

	glm::vec3 BoxCollider::GetSize() const
	{
		return _size;
	}

	bool BoxCollider::IsInitialised() const
	{
		return _physicsBoxCollider->IsInitialised();
	}

	bool BoxCollider::IsColliding(const std::shared_ptr<BoxCollider> collider, const bool& aligned)
	{
		return _physicsBoxCollider->IsColliding(collider->_physicsBoxCollider, aligned);
	}

	bool BoxCollider::IsColliding(const std::shared_ptr<MeshCollider> collider)
	{
		return _physicsBoxCollider->IsColliding(collider->_physicsMeshCollider);
	}

	glm::vec3 BoxCollider::GetCollisionResponse(const std::shared_ptr<BoxCollider> collider)
	{
		const auto colliding = [&]() { return IsColliding(collider); };

		auto amount = 0.1f;
		const auto step = 0.1f;

		const auto addToPosition = [&col = _physicsBoxCollider](auto& val) { col->SetPosition(col->GetPosition() + val); };


		while (true)
		{
			if (!colliding()) break;

			// X-axis:
			addToPosition(glm::vec3(amount, 0.0f, 0.0f));
			if (!colliding()) break;
			addToPosition(glm::vec3(-amount, 0.0f, 0.0f));
			addToPosition(glm::vec3(-amount, 0.0f, 0.0f));
			if (!colliding()) break;
			addToPosition(glm::vec3(amount, 0.0f, 0.0f));

			// Z-axis:
			addToPosition(glm::vec3(0.0f, 0.0f, amount));
			if (!colliding()) break;
			addToPosition(glm::vec3(0.0f, 0.0f, -amount));
			addToPosition(glm::vec3(0.0f, 0.0f, -amount));
			if (!colliding()) break;
			addToPosition(glm::vec3(0.0f, 0.0f, amount));

			// Y-axis:
			addToPosition(glm::vec3(0.0f, amount, 0.0f));
			if (!colliding()) break;
			addToPosition(glm::vec3(0.0f, -amount, 0.0f));
			addToPosition(glm::vec3(0.0f, -amount, 0.0f));
			if (!colliding()) break;
			addToPosition(glm::vec3(0.0f, amount, 0.0f));

			// Increment the amount by the step to increase the distance we
			// move away from the collided object:
			amount += step;
		}
		return _physicsBoxCollider->GetPosition();
	}

	glm::vec3 BoxCollider::GetCollisionResponse(const std::shared_ptr<MeshCollider> collider)
	{
		const auto colliding = [&]() { return IsColliding(collider); };

		auto amount = 0.1f;
		const auto step = 0.1f;

		const auto addToPosition = [&col = _physicsBoxCollider](auto& val) { col->SetPosition(col->GetPosition() + val); };


		while (true)
		{
			if (!colliding()) break;

			// X-axis:
			addToPosition(glm::vec3(amount, 0.0f, 0.0f));
			if (!colliding()) break;
			addToPosition(glm::vec3(-amount, 0.0f, 0.0f));
			addToPosition(glm::vec3(-amount, 0.0f, 0.0f));
			if (!colliding()) break;
			addToPosition(glm::vec3(amount, 0.0f, 0.0f));

			// Z-axis:
			addToPosition(glm::vec3(0.0f, 0.0f, amount));
			if (!colliding()) break;
			addToPosition(glm::vec3(0.0f, 0.0f, -amount));
			addToPosition(glm::vec3(0.0f, 0.0f, -amount));
			if (!colliding()) break;
			addToPosition(glm::vec3(0.0f, 0.0f, amount));

			// Y-axis:
			addToPosition(glm::vec3(0.0f, amount, 0.0f));
			if (!colliding()) break;
			addToPosition(glm::vec3(0.0f, -amount, 0.0f));
			addToPosition(glm::vec3(0.0f, -amount, 0.0f));
			if (!colliding()) break;
			addToPosition(glm::vec3(0.0f, amount, 0.0f));

			// Increment the amount by the step to increase the distance we
			// move away from the collided object:
			amount += step;
		}
		return _physicsBoxCollider->GetPosition();
	}

	//void BoxCollider::OnCollisionEnter()
	//{

	//}

	// #############################################################################
	// MeshCollider Functions:
	// #############################################################################
	void MeshCollider::Initialise()
	{
		// If the game object has a model renderer then capture all vertices from all meshes:
		if (auto modelRenderer = GetGameObject()->GetComponent<ModelRenderer>())
		{
			std::vector<glm::vec3> vertexCache;
			auto meshes = modelRenderer->_model->_renderModel->GetMeshes();
			for (auto mesh : meshes)
			{
				const auto vertices = mesh->GetVertices();
				for (auto vertex : vertices) vertexCache.push_back(vertex.Position);
			}
			_physicsMeshCollider = MimicPhysics::MeshCollider::Initialise(vertexCache);
			_physicsMeshCollider->SetPosition(GetGameObject()->Position);
			return;
		}
	}

	void MeshCollider::Start()
	{
		
	}

	void MeshCollider::Update()
	{
	
	}

	void MeshCollider::FixedUpdate()
	{
		// If the game object has a model renderer then capture all vertices from all meshes:
		if (auto modelRenderer = GetGameObject()->GetComponent<ModelRenderer>())
		{
			std::vector<glm::vec3> vertexCache;
			auto meshes = modelRenderer->_model->_renderModel->GetMeshes();
			for (auto mesh : meshes)
			{
				const auto vertices = mesh->GetVertices();
				for (auto vertex : vertices) vertexCache.push_back(vertex.Position);
			}
			_physicsMeshCollider->SetPosition(GetGameObject()->Position);
			return;
		}
	}

	bool MeshCollider::IsInitialised() const
	{
		return _physicsMeshCollider->IsInitialised();
	}

	bool MeshCollider::IsColliding(const std::shared_ptr<BoxCollider> collider)
	{
		return _physicsMeshCollider->IsColliding(collider->_physicsBoxCollider);
	}

	bool MeshCollider::IsColliding(const std::shared_ptr<MeshCollider> collider)
	{
		return _physicsMeshCollider->IsColliding(collider->_physicsMeshCollider);
	}

	glm::vec3 MeshCollider::GetCollisionResponse(const std::shared_ptr<BoxCollider> collider)
	{
		const auto colliding = [&]() { return IsColliding(collider); };

		auto amount = 0.1f;
		const auto step = 0.1f;

		const auto addToPosition = [&col = _physicsMeshCollider](auto& val) { col->SetPosition(col->GetPosition() + val); };
		

		while (true)
		{
			if (!colliding()) break;
			
			// X-axis:
			addToPosition(glm::vec3(amount, 0.0f, 0.0f));
			if (!colliding()) break;
			addToPosition(glm::vec3(-amount, 0.0f, 0.0f));
			addToPosition(glm::vec3(-amount, 0.0f, 0.0f));
			if (!colliding()) break;
			addToPosition(glm::vec3(amount, 0.0f, 0.0f));

			// Z-axis:
			addToPosition(glm::vec3(0.0f, 0.0f, amount));
			if (!colliding()) break;
			addToPosition(glm::vec3(0.0f, 0.0f, -amount));
			addToPosition(glm::vec3(0.0f, 0.0f, -amount));
			if (!colliding()) break;
			addToPosition(glm::vec3(0.0f, 0.0f, amount));

			// Y-axis:
			addToPosition(glm::vec3(0.0f, amount, 0.0f));
			if (!colliding()) break;
			addToPosition(glm::vec3(0.0f, -amount, 0.0f));
			addToPosition(glm::vec3(0.0f, -amount, 0.0f));
			if (!colliding()) break;
			addToPosition(glm::vec3(0.0f, amount, 0.0f));

			// Increment the amount by the step to increase the distance we
			// move away from the collided object:
			amount += step;
		}
		return _physicsMeshCollider->GetPosition();
	}

	glm::vec3 MeshCollider::GetCollisionResponse(const std::shared_ptr<MeshCollider> collider)
	{
		const auto colliding = [&]() { return IsColliding(collider); };

		auto amount = 0.1f;
		const auto step = 0.1f;

		const auto addToPosition = [&col = _physicsMeshCollider](auto& val) { col->SetPosition(col->GetPosition() + val); };


		while (true)
		{
			if (!colliding()) break;

			// X-axis:
			addToPosition(glm::vec3(amount, 0.0f, 0.0f));
			if (!colliding()) break;
			addToPosition(glm::vec3(-amount, 0.0f, 0.0f));
			addToPosition(glm::vec3(-amount, 0.0f, 0.0f));
			if (!colliding()) break;
			addToPosition(glm::vec3(amount, 0.0f, 0.0f));

			// Z-axis:
			addToPosition(glm::vec3(0.0f, 0.0f, amount));
			if (!colliding()) break;
			addToPosition(glm::vec3(0.0f, 0.0f, -amount));
			addToPosition(glm::vec3(0.0f, 0.0f, -amount));
			if (!colliding()) break;
			addToPosition(glm::vec3(0.0f, 0.0f, amount));

			// Y-axis:
			addToPosition(glm::vec3(0.0f, amount, 0.0f));
			if (!colliding()) break;
			addToPosition(glm::vec3(0.0f, -amount, 0.0f));
			addToPosition(glm::vec3(0.0f, -amount, 0.0f));
			if (!colliding()) break;
			addToPosition(glm::vec3(0.0f, amount, 0.0f));

			// Increment the amount by the step to increase the distance we
			// move away from the collided object:
			amount += step;
		}
		return _physicsMeshCollider->GetPosition();
	}

	//void MeshCollider::OnCollisionEnter()
	//{

	//}
}