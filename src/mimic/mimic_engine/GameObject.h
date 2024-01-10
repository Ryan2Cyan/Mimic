#pragma once
#include <mimic_utility/Logger.h>

#include <memory>
#include <vector>
#include <string>
#include <functional>
#include <glm/glm.hpp>

namespace MimicEngine
{
	struct Component;
	struct MimicCore;
	typedef std::vector<std::shared_ptr<Component>> components_vector;

	/// <summary>
	/// Primitive element that can be instantiated into the scene. Contains a position, rotation (eular angles) and scale.
	/// For additional functionality Components can be attached onto the GameObject (see GameObject::AddComponent()). Each attached
	/// Component is updated by the parent GameObject.
	/// </summary>
	struct GameObject
	{
		/// <summary>
		/// Add new component of specified type. If the component already exists on this game object, nothing will be
		/// added.
		/// </summary>
		/// <typeparam name="T">Component type.</typeparam>
		/// <returns>The newly instantiated Component type (as a shared_ptr). If the input Component type cannot be deduced, returns nullptr.</returns>
		template <typename T> std::shared_ptr<T> AddComponent()
		{
			std::shared_ptr<T> newComponent = std::make_shared<T>();
			newComponent->_self = newComponent;
			newComponent->_gameObject = _self;
		
			_components.push_back(newComponent);
			newComponent->Initialise();
			return newComponent;
		}

		/// <summary>
		/// Get component of specified type.
		/// </summary>
		/// <typeparam name="T">Component type.</typeparam>
		/// <returns>The first instance of the Component type found (as a shared_ptr), if not found returns nullptr.</returns>
		template <typename T> std::shared_ptr<T> GetComponent() const
		{
			for (auto component : _components)
			{
				std::shared_ptr<T> componentDynCast = std::dynamic_pointer_cast<T>(component);
				if (componentDynCast != nullptr) return componentDynCast;
			}
			return nullptr;
		}

		/// <summary>
		/// Set all transform parameters in one function call.
		/// </summary>
		/// <param name="position">Current position of the GameObject.</param>
		/// <param name="rotation">Current rotation (eular angles) of the GameObject.</param>
		/// <param name="scale">Current scale of the GameObject.</param>
		void Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);

		std::shared_ptr<MimicCore> GetMimicCore() const noexcept;

		/// <summary> 
		/// Called when clicked on with user cursor position in alignment with the GameObject's model, selecting the object 
		/// </summary>
		std::function<void()> OnSelected;

		/// <summary> 
		/// Called each frame whilst the object is selected. 
		/// </summary>
		std::function<void()> WhileSelected;

		/// <summary> 
		/// Called when user cursor position in alignment with the GameObject's model. 
		/// </summary>
		std::function<void()> OnHighlighted;

		/// <summary> 
		/// Called when clicked on with user cursor position in alignment with the GameObject's model whilst already selected 
		/// </summary>
		std::function<void()> OnUnselected;

		void SetActive(const bool& active);
		bool GetActive() const;

		glm::vec3 Position;
		glm::vec3 Rotation;
		glm::vec3 Scale;
		std::string Name;

	private:
		friend struct MimicCore;

		static std::shared_ptr<GameObject> Initialise();
		static std::shared_ptr<GameObject> Initialise(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);

		friend struct ModelRenderer; 
		friend struct Material; 
		friend struct PBRMaterial; 
		friend struct MeshCollider;
		friend struct InputHandler;

		void Start();
		void Update();
		void FixedUpdate();
		void GuiUpdate();
		void UpdateModelMatrix();

		components_vector _components;
		components_vector _initialisedComponents;
		glm::mat4 _modelMatrix;
		std::weak_ptr<MimicCore> _mimicCore;
		std::weak_ptr<GameObject> _self;
		bool _selected;
		bool _active;
	};
}