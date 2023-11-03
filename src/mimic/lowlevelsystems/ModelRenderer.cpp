#include "ModelRenderer.h"
#include <utility/Logger.h>
#include <lowlevelsystems/ResourceManager.h>
#include <lowlevelsystems/MimicCore.h>
#include <renderengine/Renderer.h>
#include <renderengine/Model.h>
#include <renderengine/Material.h>
#include <functional>

namespace Mimic
{
	std::shared_ptr<ModelRenderer> ModelRenderer::Initialise()
	{
		return std::make_shared<ModelRenderer>();
	}

	void ModelRenderer::Initialise(const std::string& modelFileName, const std::string& shaderFileName)
	{
		_initialised = SetModel(modelFileName) && SetShader(shaderFileName);
		_material = AddMaterial<BasicMaterial>();
		if (_initialised) MIMIC_LOG_INFO("[Mimic::ModelRenderer] Initialisation successful.");
	}

	void ModelRenderer::Initialise(const std::shared_ptr<Model>& model, const std::string& shaderFileName)
	{
		_initialised = SetModel(model) && SetShader(shaderFileName);
		_material = AddMaterial<BasicMaterial>();
		if (_initialised) MIMIC_LOG_INFO("[Mimic::ModelRenderer] Initialisation successful.");
	}

	void ModelRenderer::Initialise(const std::string& modelFileName, const std::shared_ptr<Shader>& shader)
	{
		_initialised = SetModel(modelFileName) && SetShader(shader);
		_material = AddMaterial<BasicMaterial>();
		if (_initialised) MIMIC_LOG_INFO("[Mimic::ModelRenderer] Initialisation successful.");
	}

	void ModelRenderer::Initialise(const std::shared_ptr<Model>& model, const std::shared_ptr<Shader>& shader)
	{
		_initialised = SetModel(model) && SetShader(shader);
		_material = AddMaterial<BasicMaterial>();
		if (_initialised) MIMIC_LOG_INFO("[Mimic::ModelRenderer] Initialisation successful.");
	}

	bool ModelRenderer::SetModel(const std::string& fileName)
	{
		_model = GetGameObject()->GetMimicCore()->ResourceManager->LoadResource<Model>(std::string(fileName));
		if (_model == nullptr)
		{
			MIMIC_LOG_WARNING("[ModelRenderer] % could not load model from file name \"%\".", GetGameObject()->Name, fileName);
			return false;
		}
		return true;
	}

	bool ModelRenderer::SetModel(const std::shared_ptr<Model>& model)
	{
		if (model == nullptr)
		{
			MIMIC_LOG_WARNING("[ModelRenderer] % was passed a null model.", GetGameObject()->Name);
			return false;
		}
		_model = model;
		return true;
	}

	bool ModelRenderer::SetShader(const std::string& fileName)
	{
		_shader = GetGameObject()->GetMimicCore()->ResourceManager->LoadResource<Shader>(std::string(fileName));
		if (_shader == nullptr)
		{
			MIMIC_LOG_WARNING("[ModelRenderer] % could not load shader from file name\"%\".", GetGameObject()->Name, fileName);
			return false;
		}
		return true;
	}

	bool ModelRenderer::SetShader(const std::shared_ptr<Shader>& shader)
	{
		if (shader == nullptr)
		{
			MIMIC_LOG_WARNING("[ModelRenderer] % was passed a null shader.", GetGameObject()->Name);
			return false;
		}
		_shader = shader;
		return true;
	}

	void ModelRenderer::Update() 
	{
		if (_skipUpdate) return;
		if (!_initialised)
		{
			MIMIC_LOG_WARNING("[%] Unable to draw, Model Renderer is uninitialised.", GetGameObject()->Name);
			_skipUpdate = true;
			return;
		}

		// add each mesh in the model into the draw queue:
		const std::shared_ptr<Renderer> renderer = GetGameObject()->GetMimicCore()->_renderer;
		std::function<void()> materialOnDrawLambda = [&]() { _material->OnDraw(); };
		_material->_shader = _shader;

		for (auto mesh : _model->_meshes)
		{
			// store texture id's in material:
			if(mesh->_diffuseTexture != nullptr) _material->_diffuseTexture = mesh->_diffuseTexture->_id;
			if (mesh->_specularTexture != nullptr) _material->_specularTexture = mesh->_specularTexture->_id;
			if (mesh->_normalTexture != nullptr) _material->_normalTexture = mesh->_normalTexture->_id;
			if (mesh->_heightTexture != nullptr) _material->_heightTexture = mesh->_heightTexture->_id;

			// send render object to renderer:
			 RenderObject currentRenderObject = RenderObject(
				mesh->_vertexArrayId, 
				mesh->_textures, 
				mesh->_indices, 
				_shader, 
				GetGameObject()->_modelMatrix,
				materialOnDrawLambda
			);
			renderer->AddToDrawQue(currentRenderObject);
		}
	}
}