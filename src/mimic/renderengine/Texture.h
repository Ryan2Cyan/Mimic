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
		explicit Texture(const unsigned int id, std::string type, std::string path);
		unsigned int Id;
		std::string Type;
		std::string Path;
	};
}