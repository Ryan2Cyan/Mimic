#include "GameObject.h"
#include <lowlevelsystems/Component.h>


namespace Mimic
{
	GameObject::GameObject() : Position(glm::vec3(0.0f)), Rotation(glm::vec3(0.0f)), Scale(glm::vec3(0.01f)), Name(""), _modelMatrix(glm::mat4(1.0f))
	{
		_modelMatrix = glm::scale(glm::translate(glm::mat4(1.0f), Position), Scale);
	}

	std::shared_ptr<MimicCore> GameObject::GetMimicCore() const noexcept
	{
		return _mimicCore.lock();
	}

	void GameObject::Update() noexcept
	{
		_modelMatrix = glm::translate(glm::mat4(1.0f), Position) * glm::mat4_cast(glm::quat(Rotation)) * glm::scale(glm::mat4(1.0f), Scale);

		// component updates:
		for (auto component : _components) component->Update();
	}
}