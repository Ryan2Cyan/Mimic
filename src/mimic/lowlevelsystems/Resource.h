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

		std::shared_ptr<ResourceManager> GetResourceManager() const
		{
			return _resourceManager.lock();
		}
		std::string Path;

	protected:
		friend struct ResourceManager;

		std::weak_ptr<ResourceManager> _resourceManager;
		virtual void Load(const std::string& path) = 0;
	};
}