#pragma once
#include <GL/glew.h>
#include <memory>
#include <vector>

#include "GameObject.h"

namespace Mimic
{
	// #############################################################################
    // component stuct:
    // #############################################################################
	struct GameObject;

	struct Component
	{
		std::shared_ptr<GameObject> GetGameObject() const;

	protected:
		friend struct GameObject;
		virtual void Update() = 0;
		virtual void Draw() = 0;

		std::weak_ptr<GameObject> GameObject;
		std::weak_ptr<Component> _self;
		bool _initialised = false;
	};
}