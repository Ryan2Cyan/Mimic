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
		void Initialise(const std::string& modelFileName);
		void Initialise(const std::shared_ptr<Model>& model);

		template <typename T> const std::shared_ptr<T> ReplaceMaterial()
		{
			std::shared_ptr<T> newMaterial = std::make_shared<T>();
			if (newMaterial == nullptr)
			{
				MIMIC_LOG_WARNING("[ModelRenderer]\"%\" Unable to assign material.", GetGameObject()->Name);
				return nullptr;
			}
			else
			{
				Material = newMaterial;
				if (!AttachMaterial(_model))
				{
					MIMIC_LOG_WARNING("[Mimic::ModelRenderer]\"%\" Unable to attach material to model.", GetGameObject()->Name);
					Material = std::make_shared<BasicMaterial>;
					return nullptr;
				}
				return newMaterial;
			}
		}

		template <typename T> const std::shared_ptr<T> GetMaterial() const
		{
			if (Material== nullptr)
			{
				MIMIC_LOG_WARNING("[Mimic::ModelRenderer]\"%\" Unable to fetch material, it is unassigned or nullptr.", GetGameObject()->Name);
				return nullptr;
			}
			return std::dynamic_pointer_cast<T>(Material);
		}
		void SetModel(const std::string& fileName);
		void SetModel(const std::shared_ptr<Model>& model);

		std::shared_ptr<Material> Material;

		private:
		void Update() override;
		const bool AttachMaterial(const std::shared_ptr<Model>& model);
		void ProcessModel(const std::shared_ptr<Model>& model);

		std::shared_ptr<Model>_model;
	};
}