#include "GameObject.h"
#include "Component.h"
#include "MimicCore.h"


namespace MimicEngine
{
	std::shared_ptr<GameObject> GameObject::Initialise()
	{
		auto newGameObject = std::make_shared<GameObject>();
		newGameObject->_self = newGameObject;

		// Cache game object in a list at the top of the hierarchy:
		MimicCore::AddGameObject(newGameObject);
		auto refCheck = newGameObject->_self.lock();

		// Assign Transform component variables:
		newGameObject->_modelMatrix = glm::mat4(1.0f);
		newGameObject->Transform(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
		newGameObject->UpdateModelMatrix();

		return newGameObject;
	}

	std::shared_ptr<GameObject> GameObject::Initialise(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
	{
		auto newGameObject = std::make_shared<GameObject>();
		newGameObject->_self = newGameObject;

		// Cache game object in a list at the top of the hierarchy:
		MimicCore::AddGameObject(newGameObject);

		// Assign Transform component variables:
		newGameObject->Transform(position, rotation, scale);

		return newGameObject;
	}

	void GameObject::Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
	{
		Position = position;
		Rotation = rotation;
		Scale = scale;
	}

	std::shared_ptr<MimicCore> GameObject::GetMimicCore() const noexcept
	{
		return _mimicCore.lock();
	}

	void GameObject::Start()
	{
		for (std::shared_ptr<Component> component : _components) component->Start();
	}

	void GameObject::Update()
	{
		// Update the game object's model matrix according to its position, rotation, and scale:
		UpdateModelMatrix();

		// Update all components attached to the game object:
		for (std::shared_ptr<Component> component : _components) component->Update();
	}

	void GameObject::UpdateModelMatrix()
	{
		_modelMatrix = glm::translate(glm::mat4(1.0f), Position) * glm::mat4_cast(glm::quat(Rotation)) * glm::scale(glm::mat4(1.0f), Scale);
	}
}