#include "GameObject.h"
#include <lowlevelsystems/Component.h>


namespace Mimic
{
	GameObject::GameObject() : Position(glm::vec3(0.0f)), Rotation(glm::vec3(0.0f)), Scale(glm::vec3(1.0f)), Name(""), _modelMatrix(glm::mat4(1.0f))
	{
		_modelMatrix = glm::scale(glm::translate(glm::mat4(1.0f), Position), Scale);
	}

	std::shared_ptr<MimicCore> GameObject::GetMimicCore() const noexcept
	{
		return _mimicCore.lock();
	}

	void GameObject::Start() noexcept
	{
		// only update components that are initialised:
		for (std::shared_ptr<Component> component : _components)
		{
			if (component->_initialised) _initialisedComponents.push_back(component);
		}
	}

	void GameObject::Update() noexcept
	{
		_modelMatrix = glm::translate(glm::mat4(1.0f), Position) * glm::mat4_cast(glm::quat(Rotation)) * glm::scale(glm::mat4(1.0f), Scale);

		// component updates:
		for (std::shared_ptr<Component> component : _initialisedComponents) component->Update();
	}
}