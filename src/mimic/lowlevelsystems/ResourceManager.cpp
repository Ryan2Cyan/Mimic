#include "ResourceManager.h"
#include <lowlevelsystems/Resource.h>
#include <utility>

namespace Mimic
{
	// #############################################################################
	// resource manager functions:
	// #############################################################################
	ResourceManager::ResourceManager()
	{
	}

	std::shared_ptr<ResourceManager> ResourceManager::Initialise()
	{
		return std::make_shared<ResourceManager>();
	}
}