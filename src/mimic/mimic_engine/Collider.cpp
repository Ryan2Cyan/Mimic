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

	void MeshCollider::Start()
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
		}
	}

	void MeshCollider::Update()
	{
		_physicsMeshCollider->SetOffset(GetGameObject()->Position);
	}

	bool MeshCollider::IsColliding(const std::shared_ptr<MeshCollider> collider)
	{
		if (!_physicsMeshCollider) return false;
		return _physicsMeshCollider->IsColliding(collider->_physicsMeshCollider);
	}

	/*glm::vec3 MeshCollider::GetCollisionResponse(const glm::vec3& position, const glm::vec3 size)
	{
		return glm::vec3(0.0f);
	}*/
}