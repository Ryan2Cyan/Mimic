#include "Resources.h"
#include <utility>

namespace Mimic
{
	// #############################################################################
	// resource manager functions:
	// #############################################################################
	std::shared_ptr<ResourceManager> ResourceManager::Initialise()
	{
		return std::make_shared<ResourceManager>();
	}

	template<typename T> std::shared_ptr<T> ResourceManager::LoadResource(const std::string& path) const
	{
		// check if the resource is already loaded:
		auto iterator = _loadedResources.find(path);
		if (iterator != _loadedResources.end())
		{
			std::pair<std::string, std::shared_ptr<Resource>> resultPair = *iterator;
			return resultPair.second();
		}

		// load from disk:
	}
}