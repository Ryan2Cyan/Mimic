#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include <vector>

namespace Mimic
{
	// #############################################################################
	// resource manager stuct:
	// #############################################################################
	struct MimicCore;
	struct Resource;

	struct ResourceManager 
	{
		template<typename T> std::shared_ptr<T> LoadResource(const std::string& path) const;

	private:
		std::unordered_map<std::string, std::shared_ptr<Resource>> _loadedResources;
		std::weak_ptr<MimicCore> _mimicCore;
		std::weak_ptr<ResourceManager> _self;
	};
	 
	// #############################################################################
	// resource stuct:
	// #############################################################################
	struct Resource
	{
		std::string Path;
		std::weak_ptr<ResourceManager> ResourceManager;
	};
}