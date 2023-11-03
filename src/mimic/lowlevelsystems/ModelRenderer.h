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
	struct Material;

	struct ModelRenderer : Component
	{
		static std::shared_ptr<ModelRenderer> Initialise();
		void Initialise(const std::string& modelFileName, const std::string& shaderFileName);
		void Initialise(const std::shared_ptr<Model>& model, const std::string& shaderFileName);
		void Initialise(const std::string& modelFileName, const std::shared_ptr<Shader>& shader);
		void Initialise(const std::shared_ptr<Model>& model, const std::shared_ptr<Shader>& shader);
		template <typename T> std::shared_ptr<T> AddMaterial()
		{
			std::shared_ptr<T> newMaterial = std::make_shared<T>();
			if (newMaterial == nullptr)
			{
				MIMIC_LOG_WARNING("[ModelRenderer]\"%\" Unable to assign material.", GetGameObject()->Name);
				return nullptr;
			}
			else
			{
				_material = newMaterial;
				return newMaterial;
			}
		}
		bool SetModel(const std::string& fileName);
		bool SetModel(const std::shared_ptr<Model>& model);
		bool SetShader(const std::string& fileName);
		bool SetShader(const std::shared_ptr<Shader>& shader);

		private:
		void Update() override;

		std::shared_ptr<Shader> _shader;
		std::shared_ptr<Model>_model;
		std::shared_ptr<Material>_material;
	};
}