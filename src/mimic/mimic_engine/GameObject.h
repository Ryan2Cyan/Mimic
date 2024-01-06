#pragma once
#include <mimic_utility/Logger.h>
#include <memory>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp> 

namespace MimicEngine
{
	// #############################################################################
	// GameObject stuct:
	// #############################################################################

	struct Component;
	struct MimicCore;
	typedef std::vector<std::shared_ptr<Component>> components_vector;

	struct GameObject
	{
		/// <summary> Add new component of specified type. If the component already exists on this game object, nothing will be
		/// added. </summary>
		template <typename T> std::shared_ptr<T> AddComponent()
		{
			std::shared_ptr<T> newComponent = std::make_shared<T>();
			newComponent->_self = newComponent;
			newComponent->_gameObject = _self;
		
			_components.push_back(newComponent);
			newComponent->Initialise();
			return newComponent;
		}

		/// <summary> Get component of specified type. </summary>
		template <typename T> std::shared_ptr<T> GetComponent() const
		{
			for (auto component : _components)
			{
				std::shared_ptr<T> componentDynCast = std::dynamic_pointer_cast<T>(component);
				if (componentDynCast != nullptr) return componentDynCast;
			}
			return nullptr;
		}

		/// <summary> Set all transform parameters in one function call. </summary>
		void Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);

		std::shared_ptr<MimicCore> GetMimicCore() const noexcept;

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

		void Start();
		void Update();
		void FixedUpdate();
		void UpdateModelMatrix();

		components_vector _components;
		components_vector _initialisedComponents;
		glm::mat4 _modelMatrix;
		std::weak_ptr<MimicCore> _mimicCore;
		std::weak_ptr<GameObject> _self;
		bool _cursorIntersected;
	};
}