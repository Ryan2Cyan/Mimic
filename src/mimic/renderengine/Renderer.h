#pragma once
#include <lowlevelsystems/Mimic.h>
#include <GL/glew.h>
#include <memory>
#include <string>

namespace Mimic
{	
	// #############################################################################
	// renderer stuct:
	// #############################################################################
	struct Model;
	struct Shader;

	struct ModelRenderer : Component
	{
		explicit ModelRenderer();

		std::shared_ptr<ModelRenderer> Initialise(const char* modelPath, const std::string vertexShaderPath, const std::string fragmentShaderPath);
		void Draw();

		private:
		std::shared_ptr<Shader> _shader;
		std::shared_ptr<Model>_model;
	};
}