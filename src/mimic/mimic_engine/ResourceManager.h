#pragma once
#include <mimic_utility/Logger.h>
#include <mimic_utility/FileLoader.h>

#include <unordered_map>



// Source: https://www.gameenginebook.com/ : Chapter 7: Resource and File System
namespace MimicEngine
{
	struct Resource;

	/// <summary>
	/// Responsible for loading in external game data (e.g. models, textures, and sound effects). See ResourceManager::LoadResource().
	/// </summary>
	struct ResourceManager
	{
		/// <summary>
		/// Load resouce from the assets/ directory given a specified resource filename (file extension included).
		/// Returns nullptr if the resource cannot be loaded. Returns nullptr is load was unsuccessful.
		/// </summary>
		/// <typeparam name="T">Resource type you want to load.</typeparam>
		/// <param name="fileName">Name of the file (including the extension) you want to load from within the '/assets' directory.</param>
		/// <returns>If successful, returns the loaded Resource, otherwise nullptr.</returns>
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
		/// <typeparam name="T">Resource type you want to create.</typeparam>
		/// <typeparam name="...Args">Input parameters for the Resources's Create() function. If the resource does not implement 
		/// Create(), this call will fail.</typeparam>
		/// <param name="...args"></param>
		/// <returns>If successful, returns the newly created Resource, otherwise returns nullptr.</returns>
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