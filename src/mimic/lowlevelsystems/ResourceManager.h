#pragma once
#include <utility/Logger.h>
#include <lowlevelsystems/Component.h>
#include <string>
#include <memory>
#include <unordered_map>

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


namespace Mimic
{
	// #############################################################################
	// resource manager stuct:
	// #############################################################################
	struct MimicCore;
	struct Resource;

	struct ResourceManager
	{
		explicit ResourceManager();

		// is resource loaded? yes: return, no: create new, store, then return:
		template<typename T> std::shared_ptr<T> LoadResource(const std::string& localPath)
		{
			// check if the resource is already loaded:
			std::string fullPath = AssetsDirectoryPath + localPath;
			auto iterator = _loadedResources.find(fullPath);
			if (iterator != _loadedResources.end())
			{
				auto resultPair = *iterator; 
				std::shared_ptr<T> loadedResource = std::dynamic_pointer_cast<T>(resultPair.second);
				if (loadedResource != nullptr) return loadedResource;
			}

			// load from disk:
			std::shared_ptr<T> newResource = std::make_shared<T>();
			newResource->_resourceManager = _self; // just incase a Resouce needs to load Resources itself.
			newResource->Path = localPath;
			const int success = newResource->Load(fullPath);
			if (success == -1)
			{
				MIMIC_LOG_WARNING("[ResourceManager] Invalid resource filepath attempted to load: \"%\"", localPath.c_str());
				return nullptr;
			}
			if(newResource->Name.empty()) newResource->Name = GetNameFromFilePath(fullPath);
			_loadedResources[fullPath] = newResource;
			MIMIC_LOG_INFO("[Mimic::ResourceManager] Successfully loaded resource from path: \"%\".", fullPath);
			return newResource;
		}

		const std::string AssetsDirectoryPath;

	private:
		friend struct MimicCore;

		std::shared_ptr<ResourceManager> Initialise();
		const std::string GetNameFromFilePath(const std::string& path) const noexcept;

		std::unordered_map<std::string, std::shared_ptr<Resource>> _loadedResources;
		std::weak_ptr<MimicCore> _mimicCore;
		std::weak_ptr<ResourceManager> _self;
	};
}