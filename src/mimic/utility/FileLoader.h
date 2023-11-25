#pragma once
#include <filesystem>
#include <memory>

namespace Mimic 
{
	// #############################################################################
	// file loader stuct:
	// #############################################################################
	struct FileLoader
	{
		static const std::shared_ptr<FileLoader> Initialise();
		const std::filesystem::path LocateDirectory(const std::string& directoryName);
		const std::string LocateFileInDirectory(const std::filesystem::path& directoryPath, const std::string fileName);

	private:
		std::filesystem::path _workingDirectory;
		std::filesystem::path _directorySeparators[3] = { "", "..", "..\\.." };
	};
}