#include "Collider.h"
#include "GameObject.h"
#include "ModelRenderer.h"
#include "Model.h"
#include <mimic_render/Vertex.h>
#include <mimic_render/Model.h>
#include <mimic_render/Mesh.h>
#include <mimic_utility/Logger.h>
#include <mimic_physics/Collider.h>

#include <limits>

namespace MimicEngine
{
	// #############################################################################
	// Collider Functions:
	// #############################################################################
	void Collider::SetOffset(const glm::vec3& offset)
	{
		_offset = offset;
	}

	void Collider::SetPreviousPosition(const glm::vec3& prevPos)
	{
		_previousPosition = prevPos;
	}

	glm::vec3 Collider::GetOffset() const
	{
		return _offset;
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
		_physicsBoxCollider->SetPosition(GetGameObject()->Position);
		_physicsBoxCollider->SetOffset(_offset);
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

	// #############################################################################
	// MeshCollider Functions:
	// #############################################################################
	void MeshCollider::Initialise()
	{
		std::vector<glm::vec3> vertexCache;

		// If the game object has a model renderer then capture all vertices from all meshes:
		if (auto modelRenderer = GetGameObject()->GetComponent<ModelRenderer>())
		{
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
		MIMIC_LOG_WARNING("[MimicEngine::MeshCollider] Unable to initialise without ModelRenderer component attached to GameObject.");
	}

	void MeshCollider::Start()
	{
		
	}

	void MeshCollider::Update()
	{
		_physicsMeshCollider->SetPosition(GetGameObject()->Position);
		_physicsMeshCollider->SetOffset(_offset);
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

	/*bool MeshCollider::IsColliding(const std::shared_ptr<MeshCollider> collider)
	{
		if (!_physicsMeshCollider->IsInitialised()) return false;
		
		return _physicsMeshCollider->IsColliding(collider->_physicsMeshCollider);
	}*/

	/*glm::vec3 MeshCollider::GetCollisionResponse(const glm::vec3& position, const glm::vec3 size)
	{
		return glm::vec3(0.0f);
	}*/
}