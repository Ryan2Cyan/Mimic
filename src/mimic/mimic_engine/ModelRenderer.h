#pragma once
#include "Component.h"


namespace MimicEngine
{	
	struct Model;
	struct Shader;
	struct Material;

	/// <summary>
	/// Queues a stored model (and its meshes) to be rendered. Can hold a model and a material and applies the material's
	/// properties when rendering the model. Material member is set to PBRMaterial by default.
	/// </summary>
	struct ModelRenderer : Component
	{
		/// <summary>
		/// Set the current material stored within the ModelRenderer. This material's properties will be applied
		/// to the stored model when rendered.
		/// </summary>
		/// <typeparam name="T">Material type.</typeparam>
		/// <param name="material">Material to store.</param>
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
			friend struct InputHandler;

			void Initialise() override;
			void Start() override;
			void Update() override;
			void FixedUpdate() override;

			std::shared_ptr<Model> _model; 
			std::shared_ptr<Material> _material;
	};
}