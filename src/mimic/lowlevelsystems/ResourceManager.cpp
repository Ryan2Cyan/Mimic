#include "ResourceManager.h"

namespace Mimic
{
	// #############################################################################
	// resource manager functions:
	// #############################################################################
	ResourceManager::ResourceManager() : AssetsDirectoryPath("../src/dmtk/assets/") 
	{
		_fileLoader = std::make_shared<FileLoader>();
		_assetsDirectory = _fileLoader->LocateDirectory("assets");
	}

	std::shared_ptr<ResourceManager> ResourceManager::Initialise()
	{
		return std::make_shared<ResourceManager>();
	}

	// Source https://www.youtube.com/watch?v=Z9LE3ksHEQQ
	const std::string ResourceManager::GetNameFromFilePath(const std::string& path) const noexcept
	{
		auto lastSlash = path.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = path.rfind('.');
		auto count = lastDot == std::string::npos ? path.size() - lastSlash : lastDot - lastSlash;

		return path.substr(lastSlash, count);
	}
}