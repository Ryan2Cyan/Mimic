#include "ModelRenderer.h"
#include <lowlevelsystems/ResourceManager.h>
#include <lowlevelsystems/MimicCore.h>
#include <renderengine/Model.h>
#include <renderengine/Shader.h>
#include <iostream>
#include <stdexcept>

namespace Mimic
{
	ModelRenderer::ModelRenderer() {}

	void ModelRenderer::Initialise(const char* modelPath, const std::string vertexShaderPath, const std::string fragmentShaderPath)
	{
		// initialise model:
		_model = GetGameObject()->GetMimicCore()->ResourceManager->LoadResource<Model>(std::string(modelPath));
		_model->Component = _self;

		std::shared_ptr<Shader> shader = std::make_shared<Shader>(vertexShaderPath, fragmentShaderPath);
		_shader = shader;

		_initialised = true;
	}

	void ModelRenderer::Initialise(const char* modelPath, std::shared_ptr<Shader> shader)
	{
		// initialise model:
		_model = GetGameObject()->GetMimicCore()->ResourceManager->LoadResource<Model>(std::string(modelPath));
		_model->Component = _self;

		_shader = shader;

		_initialised = true;
	}

	void ModelRenderer::Update()
	{
	
	}

	void ModelRenderer::Draw()
	{
		if (!_initialised)
		{
			std::cerr << "ERROR: GameObject [" << GetGameObject()->Name << "] ModelRenderer component is uninitialised" << std::endl;
			throw;
			return;
		}

		// assign uniforms:
		glUseProgram(_shader->ShaderProgramId);

		_shader->SetModelMatrix(GetGameObject()->_modelMatrix);
		_shader->SetViewMatrix(GetGameObject()->GetMimicCore()->CurrentCamera->_viewMatrix);
		_shader->SetProjectionMatrix(GetGameObject()->GetMimicCore()->CurrentCamera->_projectionMatrix);

		_model->Draw(_shader);
		glUseProgram(0);
	}
}