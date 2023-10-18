#pragma once
#include <GL/glew.h>
#include <memory>
#include <vector>

#include "GameObject.h"

namespace Mimic
{
	struct Component
	{
		std::shared_ptr<Component> Initialize() noexcept;

		std::weak_ptr<GameObject> GameObject;
	};
}