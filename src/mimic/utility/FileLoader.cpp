#include "FileLoader.h"
#include <utility/Logger.h>

namespace MimicUtil
{
	std::shared_ptr<FileLoader> FileLoader::Initialise() noexcept
	{
		std::shared_ptr<FileLoader> fileLoader = std::make_shared<FileLoader>();
		fileLoader->_workingDirectory = std::filesystem::current_path();
		fileLoader->_initialised = true;
		return fileLoader;
	}

	bool FileLoader::DirectoryExists(const std::filesystem::path& path)
	{
		return std::filesystem::exists(path);
	}

	std::filesystem::path FileLoader::LocateDirectory(const std::string& directoryName) const
	{
		if (!_initialised) return "";
		std::filesystem::path resultPath;
		for (auto directorySeparator : _directorySeparators)
		{
			std::filesystem::path resultPath = _workingDirectory / directorySeparator / directoryName;
			if (DirectoryExists(resultPath)) return resultPath;
		}
		MIMIC_LOG_WARNING("[RenderUtil::FileLoader] Could not find directory correlating to name: \"%\"", directoryName);
		return "";
	}

	std::string FileLoader::LocateFileInDirectory(const std::filesystem::path& directoryPath, const std::string fileName) const
	{
		if (!_initialised) return "";
		if (!DirectoryExists(directoryPath)) return "";

		std::filesystem::path resultPath;
		for (auto const& dir_entry : std::filesystem::recursive_directory_iterator{ directoryPath })
		{
			resultPath = dir_entry / fileName;
			if (DirectoryExists(resultPath)) return resultPath.generic_string();
		}
		MIMIC_LOG_WARNING("[RenderUtil::FileLoader] Could not find file \"%\" in directory correlating to name: \"%\"", fileName, directoryPath.generic_string());
		return "";
	}
}