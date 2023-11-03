#pragma once
#include <memory>
#include "GameObject.h"

namespace Mimic
{
	// #############################################################################
    // component stuct:
    // #############################################################################
	struct GameObject;

	struct Component
	{
		virtual ~Component() = default;

		std::shared_ptr<GameObject> GetGameObject() const noexcept;

	protected:
		friend struct GameObject;
		virtual void Update() = 0;

		std::weak_ptr<GameObject> GameObject;
		std::weak_ptr<Component> _self;
		bool _initialised = false;
		bool _skipUpdate = false;
	};
}