#include "GameObject.h"
#include <lowlevelsystems/Mimic.h>
#include <iostream>

namespace Mimic
{
	GameObject::GameObject() : Position(glm::vec3(0.0f)), Rotation(glm::vec3(0.0f)), Scale(glm::vec3(0.01f)), Name(""), _modelMatrix(glm::mat4(1.0f)), _componentsCount(0)
	{
		_modelMatrix = glm::scale(glm::translate(glm::mat4(1.0f), Position), Scale);
	}

	std::shared_ptr<MimicCore> GameObject::GetMimicCore() const
	{
		return _mimicCore.lock();
	}

	void GameObject::Update()
	{
		_modelMatrix = glm::translate(glm::mat4(1.0f), Position) * glm::mat4_cast(glm::quat(Rotation)) * glm::scale(glm::mat4(1.0f), Scale);

		// update all gameobject's components:
		for (unsigned int i = 0; i < _componentsCount; i++) _components[i]->Update();
	}
}