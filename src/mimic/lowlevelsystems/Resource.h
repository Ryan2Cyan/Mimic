#pragma once
#include <string>
#include <memory>

namespace Mimic
{
	// #############################################################################
	// resource stuct:
	// #############################################################################
	struct ResourceManager;

	struct Resource
	{
		std::string Path;
		std::weak_ptr<ResourceManager> ResourceManager;
	};
}