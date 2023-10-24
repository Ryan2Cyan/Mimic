#include "ModelRenderer.h"
#include <iostream>

namespace Mimic
{
	ModelRenderer::ModelRenderer() {}

	std::shared_ptr<ModelRenderer> ModelRenderer::Initialise(const char* modelPath, const std::string vertexShaderPath, const std::string fragmentShaderPath)
	{
		std::shared_ptr<ModelRenderer> modelRenderer = std::make_shared<ModelRenderer>();
		std::shared_ptr<Model> model = std::make_shared<Model>(modelPath);
		_model = model;

		std::shared_ptr<Shader> shader = std::make_shared<Shader>(vertexShaderPath, fragmentShaderPath);
		_shader = shader;
		return modelRenderer;
	}

	void ModelRenderer::Update()
	{
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