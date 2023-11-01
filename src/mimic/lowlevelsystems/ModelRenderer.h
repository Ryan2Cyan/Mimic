#pragma once
#include <lowlevelsystems/Component.h>
#include <GL/glew.h>
#include <memory>

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

		void Initialise(const char* modelPath, std::shared_ptr<Shader> shader);
		
		private:
		void Update() override;
		void Draw() override;

		std::shared_ptr<Shader> _shader;
		std::shared_ptr<Model>_model;
	};
}