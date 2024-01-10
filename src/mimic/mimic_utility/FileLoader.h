#pragma once
#include <filesystem>

namespace MimicUtility
{
	/// <summary>
	/// Given a specified file name (with file extension), can return a file path to that file name. Can also check
	/// fetch a repository path if it exists in the project.
	/// </summary>
	struct FileLoader
	{
		static std::shared_ptr<FileLoader> Initialise() noexcept;

		/// <summary>
		/// Returns true if inputted filepath exists on the users system.
		/// </summary>
		/// <param name="path">Path to a directory located within the project.</param>
		/// <returns>True if the directory exists, false otherwise.</returns>
		static bool DirectoryExists(const std::filesystem::path& path);

		/// <summary>
		/// From the working directory, loop over all directory separators and search for the input directory name. Return
		/// if located, returns empty path otherwise.
		/// </summary>
		/// <param name="directoryName">Name of directory located within the project.</param>
		/// <returns>File path to the directory. Returns an empty path if the directory does not exist.</returns>
		std::filesystem::path LocateDirectory(const std::string& directoryName) const;

		/// <summary>
		/// Given a valid directory path, will loop through all folders and files lower in hierarchy to that path, returning
		/// the path to the inputted file name if found.
		/// </summary>
		/// <param name="directoryPath">Base directory to begin the search (usually the project's '/assets' folder.</param>
		/// <param name="fileName">File name (with file extension) located within the 'directoryPath' parameter.</param>
		/// <returns></returns>
		std::string LocateFileInDirectory(const std::filesystem::path& directoryPath, const std::string fileName) const;

	private:
		std::filesystem::path _workingDirectory;
		std::filesystem::path _directorySeparators[3] = { "", "..", "..\\.." };
		bool _initialised;
	};
}