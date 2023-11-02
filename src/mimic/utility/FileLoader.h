#pragma once
#include <filesystem>

namespace Mimic 
{
	// #############################################################################
	// file loader stuct:
	// #############################################################################
	struct FileLoader
	{
		explicit FileLoader();
		const std::filesystem::path LocateDirectory(const std::string& directoryName);
		const std::filesystem::path LocateFileInDirectory(const std::filesystem::path& directoryPath, const std::string fileName);

	private:
		const std::filesystem::path _workingDirectory;
		const std::filesystem::path _directorySeparators[3] = { "", "..", "..\\.." };
	};
}