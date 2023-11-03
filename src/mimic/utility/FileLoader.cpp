#include "FileLoader.h"
#include <utility/Logger.h>

namespace Mimic
{
	FileLoader::FileLoader() : _workingDirectory(std::filesystem::current_path()) {}

	const std::filesystem::path FileLoader::LocateDirectory(const std::string& directoryName)
	{
		std::filesystem::path resultPath;
		for (auto directorySeparator : _directorySeparators)
		{
			std::filesystem::path resultPath = _workingDirectory / directorySeparator / directoryName;
			if (std::filesystem::exists(resultPath)) return resultPath;
		}
		MIMIC_LOG_WARNING("[FileLoader] Could not find directory correlating to name: \"%\"", directoryName);
		return resultPath;
	}

	const std::string FileLoader::LocateFileInDirectory(const std::filesystem::path& directoryPath, const std::string fileName)
	{
		std::filesystem::path resultPath;
		for (auto const& dir_entry : std::filesystem::recursive_directory_iterator{ directoryPath })
		{
			resultPath = dir_entry / fileName;
			if (std::filesystem::exists(resultPath)) return resultPath.generic_string();
		}
		MIMIC_LOG_WARNING("[FileLoader] Could not find file \"%\" in directory correlating to name: \"%\"", fileName, directoryPath.generic_string());
		return resultPath.generic_string();
	}
}