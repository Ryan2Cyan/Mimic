#pragma once
#include <filesystem>
#include <memory>

namespace MimicUtil
{
	// #############################################################################
	// file loader stuct:
	// #############################################################################
	struct FileLoader
	{
		static const std::shared_ptr<FileLoader> Initialise() noexcept;
		const std::filesystem::path LocateDirectory(const std::string& directoryName) const;
		const std::string LocateFileInDirectory(const std::filesystem::path& directoryPath, const std::string fileName) const;

	private:
		std::filesystem::path _workingDirectory;
		std::filesystem::path _directorySeparators[3] = { "", "..", "..\\.." };
	};
}