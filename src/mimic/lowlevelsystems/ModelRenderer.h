#pragma once
#include <lowlevelsystems/Component.h>
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
		static std::shared_ptr<ModelRenderer> Initialise();
		void Initialise(const std::string& modelFileName, const std::string& shaderFileName);
		void Initialise(const std::shared_ptr<Model>& model, const std::string& shaderFileName);
		void Initialise(const std::string& modelFileName, const std::shared_ptr<Shader>& shader);
		void Initialise(const std::shared_ptr<Model>& model, const std::shared_ptr<Shader>& shader);
		bool SetModel(const std::string& fileName);
		bool SetModel(const std::shared_ptr<Model>& model);
		bool SetShader(const std::string& fileName);
		bool SetShader(const std::shared_ptr<Shader>& shader);

		private:
		void Update() override;

		std::shared_ptr<Shader> _shader;
		std::shared_ptr<Model>_model;
	};
}