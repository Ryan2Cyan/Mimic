#include "ResourceManager.h"

namespace MimicEngine
{
	// #############################################################################
	// Resource Manager functions:
	// #############################################################################

	std::shared_ptr<ResourceManager> ResourceManager::Initialise()
	{
		std::shared_ptr<ResourceManager> resourceManager = std::make_shared<ResourceManager>();
		resourceManager->_fileLoader = MimicUtility::FileLoader::Initialise();
		resourceManager->_assetsDirectory = resourceManager->_fileLoader->LocateDirectory("assets");
		return resourceManager;
	}

	// Source: https://www.youtube.com/watch?v=Z9LE3ksHEQQ
	const std::string ResourceManager::GetNameFromFilePath(const std::string& path) const noexcept
	{
		auto lastSlash = path.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = path.rfind('.');
		auto count = lastDot == std::string::npos ? path.size() - lastSlash : lastDot - lastSlash;

		return path.substr(lastSlash, count);
	}
}