#include "BoxCollider.h"
#include "GameObject.h"
#include "ModelRenderer.h"
#include "Model.h"
#include <mimic_render/Model.h>
#include <mimic_render/Mesh.h>
#include <mimic_utility/Logger.h>

#include <limits>

namespace MimicEngine
{

	void MeshCollider::Start()
	{
		// If the game object has a model renderer then capture all vertices
		// from all meshes:
		if (auto modelRenderer = GetGameObject()->GetComponent<ModelRenderer>())
		{
			auto meshes = modelRenderer->_model->_renderModel->GetMeshes();
			for (auto mesh : meshes)
			{
				const auto vertices = mesh->GetVertices();
				for (auto vertex : vertices) _vertices.push_back(vertex.Position);
			}
		}
	}

	void MeshCollider::Update()
	{

	}

	bool MeshCollider::IsColliding(const glm::vec3& position, const glm::vec3 size)
	{
		return false;
	}

	glm::vec3 MeshCollider::GetCollisionResponse(const glm::vec3& position, const glm::vec3 size)
	{
		return glm::vec3(0.0f);
	}

	glm::vec3 MeshCollider::GetFarthestPoint(const glm::vec3 direction) const
	{
		// Add position onto each point to get it's world pos:
		auto farthestPoint = glm::vec3(0);
		float max = std::numeric_limits<float>::min();

		// Loop through all vertices, check the dot product with the direction,
		// and cache result if larger than the last, whilst caching the furthest
		// point:
		for (auto vertex : _vertices)
		{
			const float projection = glm::dot(vertex, direction);
			if (projection > max)
			{
				farthestPoint = vertex;
				max = projection;
			}
		}
		return farthestPoint + GetGameObject()->Position;
	}
}