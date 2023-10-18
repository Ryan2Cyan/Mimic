#include "Component.h"
#include <fstream>
#include <iostream>


namespace Mimic
{
	// component: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	std::shared_ptr<Component> Component::Initialize() noexcept { return std::make_shared<Component>(); }
}