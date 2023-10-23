#pragma once
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp> 

namespace Mimic
{
	struct Component;
	struct MimicCore;

	struct GameObject
	{
		explicit GameObject();

		template <typename T> std::shared_ptr<T> AddComponent()
		{
			std::shared_ptr<T> newComponent = std::make_shared<T>();
			newComponent->_self = newComponent;
			newComponent->GameObject = _self;

			_components.push_back(newComponent);
			_componentsCount++;
			return newComponent;
		}
		std::shared_ptr<MimicCore> GetMimicCore() const;

		glm::vec3 Position;
		glm::vec3 Rotation;
		glm::vec3 Scale;

	private:
		friend struct MimicCore;
		friend struct Component;
		friend struct ModelRenderer;

		void Update();

		std::vector<std::shared_ptr<Component>> _components;
		glm::mat4 _modelMatrix;
		std::weak_ptr<MimicCore> _mimicCore;
		std::weak_ptr<GameObject> _self;
		unsigned int _componentsCount;
	};
}