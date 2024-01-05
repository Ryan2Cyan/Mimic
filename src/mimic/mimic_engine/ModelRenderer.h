#pragma once
#include "Component.h"

#include <memory>
#include <string>
#include <functional>

namespace MimicEngine
{	
	// #############################################################################
	// Renderer Struct:
	// #############################################################################
	struct Model;
	struct Shader;
	struct Material;

	struct ModelRenderer : Component
	{

		template <typename T> void SetMaterial(const std::shared_ptr<T>& material)
		{
			const auto gameObjectParent = GetGameObject();
			if (material == nullptr)
			{
				MIMIC_LOG_WARNING("[MimicEngine::ModelRenderer]\"%\" unable to assign uninitialised material.", gameObjectParent->Name);
				return;
			}
			_material = material;
			_material->_gameObject = gameObjectParent;
		}

		template <typename T> const std::shared_ptr<T> GetMaterial() const
		{
			if (_material == nullptr)
			{
				MIMIC_LOG_WARNING("[MimicEngine::ModelRenderer]\"%\" Unable to get unassigned material.", GetGameObject()->Name);
				return nullptr;
			}
			return std::dynamic_pointer_cast<T>(_material);
		}

		void SetModel(const std::shared_ptr<Model>& model);

		private:
			friend struct MeshCollider;
			friend struct GameObject; 

			void Initialise() override;
			void Start() override;
			void Update() override;
			void FixedUpdate() override;

			std::shared_ptr<Model> _model; 
			std::shared_ptr<Material> _material;

	};
}