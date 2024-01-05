#pragma once
#include <memory>
#include "GameObject.h"

namespace MimicEngine
{
	// #############################################################################
    // Component stuct:
    // #############################################################################
	struct GameObject;

	struct Component
	{
		virtual ~Component() = default;

		std::shared_ptr<GameObject> GetGameObject() const noexcept;

	protected:
		friend struct GameObject;
		
		virtual void Initialise() = 0;
		virtual void Start() = 0;
		virtual void Update() = 0;

		std::weak_ptr<GameObject> _gameObject;
		std::weak_ptr<Component> _self;
		bool _initialised = false;
	};
}