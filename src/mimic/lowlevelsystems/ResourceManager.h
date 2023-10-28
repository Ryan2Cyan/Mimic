#pragma once
#include <lowlevelsystems/Component.h>
#include <string>
#include <memory>
#include <unordered_map>

namespace Mimic
{
	// #############################################################################
	// resource manager stuct:
	// #############################################################################
	struct MimicCore;
	struct Resource;

	struct ResourceManager
	{
		explicit ResourceManager();

		// is resource loaded? yes: return, no: create new, store, then return:
		template<typename T> std::shared_ptr<T> LoadResource(const std::string& path)
		{
			// check if the resource is already loaded:
			auto iterator = _loadedResources.find(path);
			if (iterator != _loadedResources.end())
			{
				auto resultPair = *iterator; 
				std::shared_ptr<T> loadedResource = std::dynamic_pointer_cast<T>(resultPair.second);
				if (loadedResource != nullptr) return loadedResource;
			}

			// load from disk:
			std::shared_ptr<T> newResource = std::make_shared<T>();
			newResource->_resourceManager = _self; // just incase a Resouce needs to load Resources itself.
			newResource->Load(path);
			newResource->Path = path;
			_loadedResources[path] = newResource;
			return newResource;
		}

	private:
		friend struct MimicCore;

		std::shared_ptr<ResourceManager> Initialise();
		std::unordered_map<std::string, std::shared_ptr<Resource>> _loadedResources;
		std::weak_ptr<MimicCore> _mimicCore;
		std::weak_ptr<ResourceManager> _self;
	};
}