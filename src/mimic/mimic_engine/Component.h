#pragma once
#include <memory>
#include "GameObject.h"

namespace MimicEngine
{
	struct GameObject;

	/// <summary>
	/// Component: Base struct for engine components. Components are attached onto GameObjects (see GameObject::AddComponent()). Each
	/// component can execute an Initialise, Start, Update, FixedUpdate, and GuiUpdate function, called by the attached GameObject parent.
	/// </summary>
	struct Component
	{
		virtual ~Component() = default;

		std::shared_ptr<GameObject> GetGameObject() const noexcept;

	protected:
		friend struct GameObject;
		
		virtual void Initialise() = 0;
		virtual void Start() = 0;
		virtual void Update() = 0;
		virtual void FixedUpdate() {}
		virtual void GuiUpdate() {}

		std::weak_ptr<GameObject> _gameObject;
		std::weak_ptr<Component> _self;
		bool _initialised = false;
	};
}