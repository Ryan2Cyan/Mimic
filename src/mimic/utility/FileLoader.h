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
		static std::shared_ptr<FileLoader> Initialise() noexcept;

		/// <summary>
		/// Returns true if inputted filepath exists on the users system.
		/// </summary>
		static bool DirectoryExists(const std::filesystem::path& path);

		/// <summary>
		/// From the working directory, loop over all directory separators and search for the input directory name. Return
		/// if located, returns empty path otherwise.
		/// </summary>
		std::filesystem::path LocateDirectory(const std::string& directoryName) const;

		/// <summary>
		/// Given a valid directory path, will loop through all folders and files lower in hierarchy to that path, returning
		/// the path to the inputted file name if found.
		/// </summary>
		std::string LocateFileInDirectory(const std::filesystem::path& directoryPath, const std::string fileName) const;

	private:
		std::filesystem::path _workingDirectory;
		std::filesystem::path _directorySeparators[3] = { "", "..", "..\\.." };
		bool _initialised;
	};
}