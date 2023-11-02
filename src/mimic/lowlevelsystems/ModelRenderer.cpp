#include "ModelRenderer.h"
#include <utility/Logger.h>
#include <lowlevelsystems/ResourceManager.h>
#include <lowlevelsystems/MimicCore.h>
#include <renderengine/Model.h>
#include <renderengine/Shader.h>

namespace Mimic
{
	ModelRenderer::ModelRenderer() {}

	void ModelRenderer::Initialise(const char* modelPath, std::shared_ptr<Shader> shader)
	{
		_initialised = true;

		// initialise model:
		_model = GetGameObject()->GetMimicCore()->ResourceManager->LoadResource<Model>(std::string(modelPath));
		if (_model == nullptr)
		{
			MIMIC_LOG_WARNING("[%] Model Renderer could not load model from model path \"%\".", GetGameObject()->Name, modelPath);
			_initialised = false;
		}
		else _model->Component = _self;

		if (shader == nullptr)
		{
			MIMIC_LOG_WARNING("[%] Model Renderer passed invalid shader.", GetGameObject()->Name);
			_initialised = false;
		}
		else _shader = shader;

		if (_initialised) MIMIC_LOG_INFO("[Mimic::ModelRenderer] Initialisation successful.");
	}

	void ModelRenderer::Update()
	{
	
	}

	void ModelRenderer::Draw()
	{
		if (_skipDraw) return;
		if (!_initialised)
		{
			MIMIC_LOG_WARNING("[%] Unable to draw, Model Renderer is uninitialised.", GetGameObject()->Name);
			_skipDraw = true;
			return;
		}
		// need to add each mesh in each model to the draw queue here:
		_model->Draw(_shader);
	}
}