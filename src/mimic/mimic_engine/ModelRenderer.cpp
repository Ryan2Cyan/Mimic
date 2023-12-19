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
	std::shared_ptr<ModelRenderer> ModelRenderer::Initialise()
	{
		return std::make_shared<ModelRenderer>();
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
		
		// Set OnDrawLambda:
		std::function<void()> onDrawLambda = [&]() { _material->OnDraw(); };

		for (auto mesh : _model->_renderModel->GetMeshes())
		{
			MimicCore::_renderer->AddToDrawQueue(
				MimicRender::RenderObject::Initialise(
					mesh->GetVertexArrayId(),
					mesh->GetDataSize(),
					_material->_shader.lock()->_renderShader,
					GetGameObject()->_modelMatrix,
					onDrawLambda
				)
			);
		}
	}
}