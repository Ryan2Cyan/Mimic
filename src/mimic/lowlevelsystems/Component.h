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
		virtual void Update() = 0;
		virtual void Draw() = 0;
		
		bool _initialised = false;

	protected:
		friend struct GameObject;

		std::weak_ptr<GameObject> GameObject;
		std::weak_ptr<Component> _self;
	};
}