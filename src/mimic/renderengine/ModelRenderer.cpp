#include "ModelRenderer.h"
#include <iostream>
#include <stdexcept>

namespace Mimic
{
	ModelRenderer::ModelRenderer() {}

	void ModelRenderer::Initialise(const char* modelPath, const std::string vertexShaderPath, const std::string fragmentShaderPath)
	{
		std::shared_ptr<Model> model = std::make_shared<Model>(modelPath);
		_model = model;

		std::shared_ptr<Shader> shader = std::make_shared<Shader>(vertexShaderPath, fragmentShaderPath);
		_shader = shader;

		_initialised = true;
	}

	void ModelRenderer::Update()
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
		// KARSTEN ADVICE: Grab current camera here:
		_shader->SetViewMatrix(GetGameObject()->GetMimicCore()->MainCamera->_viewMatrix);
		_shader->SetProjectionMatrix(GetGameObject()->GetMimicCore()->MainCamera->_projectionMatrix);

		_model->Draw(_shader);
		glUseProgram(0);
	}
}