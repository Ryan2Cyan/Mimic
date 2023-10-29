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
		unsigned int Id;
		std::string Type;
		void Load(const std::string& path) override;

	protected:

	private:
	};
}