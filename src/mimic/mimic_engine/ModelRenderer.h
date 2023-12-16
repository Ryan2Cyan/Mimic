#pragma once
#include "Component.h"

#include <memory>
#include <string>

namespace MimicEngine
{	
	// #############################################################################
	// Renderer Stuct:
	// #############################################################################
	struct Model;
	struct Shader;
	struct Material;


	struct ModelRenderer : Component
	{
		static std::shared_ptr<ModelRenderer> Initialise();

		template <typename T> void SetMaterial(const std::shared_ptr<T>& material)
		{
			if (material == nullptr)
			{
				MIMIC_LOG_WARNING("[MimicEngine::ModelRenderer]\"%\" unable to assign uninitialised material.", GetGameObject()->Name);
				return;
			}
			auto downCastMaterial = std::dynamic_pointer_cast<T>(material);
			if (downCastMaterial == nullptr)
			{
				MIMIC_LOG_WARNING("[MimicEngine::ModelRenderer]\"%\" unable to assign material, unrecognised Material type.", GetGameObject()->Name);
				return;
			}
			_material = downCastMaterial;
		}

		template <typename T> const std::shared_ptr<T> GetMaterial() const
		{
			if (_material == nullptr)
			{
				MIMIC_LOG_WARNING("[MimicEngine::ModelRenderer]\"%\" Unable to get unassigned material.", GetGameObject()->Name);
				return nullptr;
			}
			auto downCastMaterial = std::dynamic_pointer_cast<T>(_material);
			if (downCastMaterial == nullptr)
			{
				MIMIC_LOG_WARNING("[MimicEngine::ModelRenderer]\"%\" Unable to get material, unrecognised Material type", GetGameObject()->Name);
				return nullptr;
			}
			return std::dynamic_pointer_cast<T>(Material);
		}

		void SetModel(const std::shared_ptr<Model>& model);

		private:
		void Start() override;
		void Update() override;

		 std::shared_ptr<Model>_model;
		 std::shared_ptr<Material> _material;
	};
}