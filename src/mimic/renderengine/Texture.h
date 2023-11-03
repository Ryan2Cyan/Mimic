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
		const int Load(const std::string& path) override;

	private:
		friend struct ModelRenderer;
		friend struct Renderer;
		friend struct Mesh;
		friend struct Model;

		unsigned int _id;
		std::string _type;
	};
}