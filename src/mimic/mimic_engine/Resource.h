#pragma once
#include <string>
#include <memory>

namespace MimicEngine
{
	// #############################################################################
	// resource stuct:
	// #############################################################################
	struct ResourceManager;

	struct Resource
	{
		virtual ~Resource() = default;

		std::shared_ptr<ResourceManager> GetResourceManager() const noexcept
		{
			return _resourceManager.lock();
		}

		std::string Name;
		std::string Path;

	protected:
		friend struct ResourceManager;

		// Return true on successful load, false otherwise:
		virtual bool Load(const std::string& path) = 0; 

		std::weak_ptr<ResourceManager> _resourceManager;
		bool _initialised;
	};
}