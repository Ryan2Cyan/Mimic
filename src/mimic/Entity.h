#pragma once
#include <memory>
#include <vector>

namespace Mimic
{
	struct Component;

	struct Entity
	{
		template <typename T> std::shared_ptr<T> AddComponent()
		{
			std::shared_ptr<T> newComponent = std::make_shared<T>();
			newComponent->Entity = Self;
			newComponent->Initialise();

			_components.push_back(newComponent);
			return newComponent;
		}

		std::weak_ptr<Entity> Self;

	private:
		std::vector<std::shared_ptr<Component>> _components;
	};
}