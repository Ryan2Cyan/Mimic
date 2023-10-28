#pragma once
#include <lowlevelsystems/Resource.h>
#include <string>


namespace Mimic
{
	// #############################################################################
	// texture stuct:
	// #############################################################################
	struct Texture : Resource
	{
		Texture();
		Texture(const unsigned int id, const std::string type, const std::string path);

		unsigned int Id;
		std::string Type;
		std::string Path;
		void Load(const std::string& path) override;
		unsigned int LoadTextureFromFile(const char* path, const std::string& directory, bool gamma);

	protected:

	private:
	};
}