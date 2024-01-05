#include "ModelRenderer.h"
#include "ResourceManager.h"
#include "MimicCore.h"
#include "Material.h"
#include "Model.h"
#include "Shader.h"
#include <mimic_utility/Logger.h>
#include <mimic_render/Renderer.h>
#include <mimic_render/Mesh.h>

#include <GLM/glm.hpp>

namespace MimicEngine
{
	void ModelRenderer::Initialise()
	{
		_material = PBRMaterial::Initialise(_gameObject);
	}

	void ModelRenderer::SetModel(const std::shared_ptr<Model>& model)
	{
		if (model == nullptr)
		{
			MIMIC_LOG_WARNING("[MimicEngine::ModelRenderer] \"%\" unable to assign uninitialised model.", GetGameObject()->Name);
			return;
		}
		_model = model;
	}

	void ModelRenderer::Start()
	{
		_initialised = _material != nullptr && _model != nullptr;
	}

	void ModelRenderer::Update() 
	{
		if (!_initialised) return;
		const auto gameObject = GetGameObject();

		// Temporarily updating model to game object's model matrix.
		_model->_renderModel->UpdateModelMatrix(gameObject->Position, gameObject->Rotation, gameObject->Scale);

		// Using both a valid model and material, create a render object, of which is passed into MimicCore's
		// primary renderer to be drawn.
		for (auto mesh : _model->_renderModel->GetMeshes())
		{
			gameObject->GetMimicCore()->_renderer->AddToDrawQueue(MimicRender::RenderObject::Initialise(
					mesh->GetVertexArrayId(),
					mesh->GetDataSize(),
					_material->_shader.lock()->_renderShader,
					gameObject->_modelMatrix,
					[&mat = _material]() { mat->OnDraw(); }
				)
			);
		}
	}
}