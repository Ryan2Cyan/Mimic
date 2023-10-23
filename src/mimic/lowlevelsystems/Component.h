#pragma once
#include <GL/glew.h>
#include <memory>
#include <vector>

#include "GameObject.h"

namespace Mimic
{
	struct GameObject; 
	struct Component
	{
		virtual void Update() = 0;
		std::shared_ptr<GameObject> GetGameObject() const;

		std::weak_ptr<GameObject> GameObject;
	};
}