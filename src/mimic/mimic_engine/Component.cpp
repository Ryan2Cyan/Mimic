#include "Component.h"

namespace MimicEngine
{
	std::shared_ptr<GameObject> Component::GetGameObject() const noexcept
	{
		return _gameObject.lock();
	}
}