#include "Renderer.h"
#include <iostream>

namespace Mimic
{
	ModelRenderer::ModelRenderer() {}

	std::shared_ptr<ModelRenderer> ModelRenderer::Initialise(const char* modelPath, const std::string vertexShaderPath, const std::string fragmentShaderPath)
	{
		return std::make_shared<ModelRenderer>();
	}

	void ModelRenderer::Draw()
	{
		/*_model.Draw(_shader);*/
	}
}