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
		virtual ~Resource() = default;

		std::shared_ptr<ResourceManager> GetResourceManager() const noexcept
		{
			return _resourceManager.lock();
		}

		std::string Name;
		std::string Path;

	protected:
		friend struct ResourceManager;

		std::weak_ptr<ResourceManager> _resourceManager;

		// Return -1 on failure, 0 on success:
		virtual const int Load(const std::string& path) = 0; 
	};
}