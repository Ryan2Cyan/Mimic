#include "ModelRenderer.h"
#include <utility/Logger.h>
#include <lowlevelsystems/ResourceManager.h>
#include <lowlevelsystems/MimicCore.h>
#include <renderengine/Renderer.h>
#include <renderengine/Model.h>
#include <renderengine/Material.h>
#include <functional>

// need to provide a way for the user to edit some parameters inside the material (e.g. albedo).

namespace Mimic
{
	std::shared_ptr<ModelRenderer> ModelRenderer::Initialise()
	{
		return std::make_shared<ModelRenderer>();
	}

	void ModelRenderer::Initialise(const std::string& modelFileName)
	{
		Material = std::make_shared<PBRMaterial>();
		SetModel(modelFileName);
		if (_initialised) MIMIC_LOG_INFO("[Mimic::ModelRenderer] Initialisation successful.");
	}

	void ModelRenderer::Initialise(const std::shared_ptr<Model>& model)
	{
		Material = std::make_shared<PBRMaterial>();
		SetModel(model);
		if (_initialised) MIMIC_LOG_INFO("[Mimic::ModelRenderer] Initialisation successful.");
	}

	void ModelRenderer::SetModel(const std::string& fileName)
	{
		ProcessModel(MimicCore::ResourceManager->LoadResource<Model>(std::string(fileName)));
	}

	void ModelRenderer::SetModel(const std::shared_ptr<Model>& model)
	{
		ProcessModel(model);
	}

	void ModelRenderer::ProcessModel(const std::shared_ptr<Model>& model)
	{
		if (!AttachMaterial(model))
		{
			MIMIC_LOG_WARNING("[Mimic::ModelRenderer]\"%\" Unable to attach material to model.", GetGameObject()->Name);
			_initialised = false;
			return;
		}
		_model = model;
		_initialised = true;
	}

	const bool ModelRenderer::AttachMaterial(const std::shared_ptr<Model>& model)
	{
		if (model == nullptr)
		{
			MIMIC_LOG_WARNING("[Mimic::ModelRenderer] \"%\" attempted to set it's material without a valid model.", GetGameObject()->Name);
			return false;
		}

		if (model->_materialTextures.size() <= 0) MIMIC_LOG_WARNING("[Mimic::ModelRenderer] \"%\" Model has no loaded textures.", GetGameObject()->Name);
		else
		{
			for (auto texture : model->_materialTextures)
			{
				if (texture->_type == "diffuse") Material->SetDiffuse(texture);
				if (texture->_type == "specular") Material->SetSpecular(texture);
				if (texture->_type == "normal") Material->SetNormal(texture);
				if (texture->_type == "height") Material->SetHeight(texture);
			}
		}

		if (Material->_shader.expired())
		{
			MIMIC_LOG_WARNING("[Mimic::ModelRenderer] \"%\" material failed to load shader.", GetGameObject()->Name);
			return false;
		}

		return true;
	}

	void ModelRenderer::Update() 
	{
		if (_skipUpdate) return;
		if (!_initialised)
		{
			MIMIC_LOG_WARNING("[Mimic::ModelRenderer] \"%\" is unable to draw, model is uninitialised.", GetGameObject()->Name);
			_skipUpdate = true;
			return;
		}

		// add each mesh in the model into the draw queue:
		std::function<void()> materialOnDrawLambda = [&]() { this->Material->OnDraw(); };

		for (auto mesh : _model->_meshes)
		{
			// send render object to renderer:
			MimicCore::_renderer->AddToDrawQue(RenderObject(
				mesh->_vertexArrayId,
				mesh->_indices,
				Material->_shader.lock(),
				GetGameObject()->_modelMatrix,
				materialOnDrawLambda
			));
		}
	}
}