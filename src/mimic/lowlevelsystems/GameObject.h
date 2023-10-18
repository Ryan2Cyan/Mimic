#pragma once
#include <memory>
#include <vector>
#include <GLM/glm.hpp>

namespace Mimic
{
	struct Component;

	struct GameObject
	{
		template <typename T> std::shared_ptr<T> AddComponent()
		{
			std::shared_ptr<T> newComponent = std::make_shared<T>();
			newComponent->GameObject = Self;

			_components.push_back(newComponent);
			return newComponent;
		}

		std::weak_ptr<GameObject> Self;
		glm::vec3 Position;
		glm::vec3 Rotation;
		glm::vec3 Scale;

	private:
		std::vector<std::shared_ptr<Component>> _components;
	};
}