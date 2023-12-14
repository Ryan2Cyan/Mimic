#pragma once
#include <mimic_utility/Logger.h>
#include <mimic_utility/FileLoader.h>

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <filesystem>


// Source: https://www.gameenginebook.com/ : Chapter 7: Resource and File System

// File System:
// 1.) manipulate file names and paths.
// 2.) open, close, read, or write to individual files.
// 3.) scan contents of a directory.
// 4.) handle asynchronous file I/O requests (for streaming).

// OS Differences:
//     OS          PathComponentSeparator    CaseSensitive?     VolumeSeparator     CurrentWorkingDirectory     CurrentWorkingVolume
//     UNIX:                 /					    N                 None                     Y                          N
//     Old Windows:          \						Y          [Single_Letter] + :             Y                          Y
//     Recent Windows:     / or \					Y		   [Single_Letter] + :   		   Y                          Y
//     MS-DOS                /                      Y          [Single_Letter] + :             Y                          Y
//     Mac OS 8 & 9:         :						N			      None                     Y                          N
//     Mac OS X:             /						N                 None                     Y                          N
// The final dot '.' in any path denotes the file extension (e.g. ".exe" or ".pdf").

// Search Paths: 
// a string containing a list of paths, each separated by a special character (e.g. ':' or ';'). A list of
// directories that should be searched to find an asset.

// Basic File I/O:
// C standard library provides a buffered & unbuffered API ('stream I/O'): 
// Buffered: https://learn.microsoft.com/en-us/cpp/c-runtime-library/stream-i-o?view=msvc-170&redirectedfrom=MSDN
// Unbuffered: https://learn.microsoft.com/en-us/cpp/c-runtime-library/low-level-i-o?view=msvc-170&redirectedfrom=MSDN

// Wrapping I/O:
// Can guarantee identical behaviour across all target platforms.
// Can simplify down to only the functions needed by the engine.
// Can extend functionality.
// Synchronous File I/O: The program making the I/O request must wait until the data has been transferred before continuing.
// Asynchronous File I/O:


namespace MimicEngine
{
	// #############################################################################
	// Resource Manager stuct:
	// #############################################################################
	struct Resource;

	struct ResourceManager
	{
		/// <summary>
		/// Load resouce from the assets/ directory given a specified resource filename (file extension included).
		/// Returns nullptr if the resource cannot be loaded. Returns nullptr is load was unsuccessful.
		/// </summary>
		template<typename T> std::shared_ptr<T> LoadResource(const std::string& fileName)
		{
			// Convert retrieve the file path from the inputted file name:
			const std::string filePath = _fileLoader->LocateFileInDirectory(_assetsDirectory, fileName);

			// Check if the resource is already cached in the loaded resources map:
			auto iterator = _loadedResources.find(filePath);
			if (iterator != _loadedResources.end())
			{
				auto resultPair = *iterator; 
				std::shared_ptr<T> loadedResource = std::dynamic_pointer_cast<T>(resultPair.second);
				if (loadedResource != nullptr) return loadedResource;
			}

			// Create new resource and load its data from disk:
			std::shared_ptr<T> newResource = std::make_shared<T>();

			// Store a reference to the resource manager, just incase this resouce needs to 
			// load Resources itself:
			newResource->_resourceManager = _self;
			newResource->Path = fileName;
			if (newResource->Name.empty()) newResource->Name = GetNameFromFilePath(filePath);
			const int success = newResource->Load(filePath);
			newResource->_initialised = success;
			if (!success)
			{
				MIMIC_LOG_WARNING("[Mimic::ResourceManager] Invalid resource filepath attempted to load: \"%\"", fileName.c_str());
				return nullptr;
			}

			// Cache newly loaded resource:
			_loadedResources[filePath] = newResource;
			MIMIC_LOG_INFO("[Mimic::ResourceManager] Successfully loaded resource from path: \"%\".", filePath);

			return newResource;
		}

		/// <summary>
		/// Create, initialise, and return new resource. Returns nullptr is creation was unsuccessful.  
		/// </summary>
		template<typename T, typename... Args> std::shared_ptr<T> CreateResource(Args... args)
		{
			std::shared_ptr<T> newResource = std::make_shared<T>();
			newResource->_resourceManager = _self;

			const int success = newResource->Create(args...);
			if (success == -1)
			{
				MIMIC_LOG_WARNING("[Mimic::ResourceManager] Unable to create new resource.");
				return nullptr;
			}

			newResource->Name = GetNameFromFilePath("NewResource" + _createdResources.size());
			_createdResources.push_back(newResource);
			MIMIC_LOG_INFO("[Mimic::ResourceManager] Successfully created new resource");
			return newResource;
		}

		const std::string AssetsDirectoryPath;

	private:
		friend struct MimicCore;
		friend struct CubeMap;

		static std::shared_ptr<ResourceManager> Initialise();
		const std::string GetNameFromFilePath(const std::string& path) const noexcept;

		std::unordered_map<std::string, std::shared_ptr<Resource>> _loadedResources;
		std::vector<std::shared_ptr<Resource>> _createdResources;
		std::shared_ptr<MimicUtility::FileLoader> _fileLoader;
		std::filesystem::path _assetsDirectory;
		std::weak_ptr<MimicCore> _mimicCore;
		std::weak_ptr<ResourceManager> _self;
	};
 }