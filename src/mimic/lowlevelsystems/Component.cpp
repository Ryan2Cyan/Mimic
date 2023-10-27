#include "Component.h"
#include <fstream>
#include <iostream>


namespace Mimic
{
	std::shared_ptr<GameObject> Component::GetGameObject() const 
	{
		return GameObject.lock();
	}
}