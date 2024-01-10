#pragma once
#include <string>
#include <memory>

namespace MimicEngine
{
	struct ResourceManager;

	/// <summary>
	/// Base struct for all resources. Resources can be loaded by the ResourceManager struct by passing in a 
	/// file name (with the file extension). As long as the filename matches a file within the '/assets' folder
	/// it can be loaded.
	/// </summary>
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