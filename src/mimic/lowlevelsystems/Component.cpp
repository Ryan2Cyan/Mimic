#include "Component.h"


namespace Mimic
{
	std::shared_ptr<GameObject> Component::GetGameObject() const noexcept
	{
		return GameObject.lock();
	}
}