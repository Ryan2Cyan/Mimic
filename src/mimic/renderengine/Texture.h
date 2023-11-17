#pragma once
#include <lowlevelsystems/Resource.h>
#include <string>
#include <GL/glew.h>
#include <GLM/glm.hpp>


namespace Mimic
{
	// #############################################################################
	// texture stuct:
	// #############################################################################
	struct Texture : Resource
	{
		const int Load(const std::string& path) override;
		const int Create(const glm::ivec2& aspectRatio, const GLenum& targetType, const GLint& internalFormat, const GLenum& format, const GLenum& dataType);

	private:
		friend struct ModelRenderer;
		friend struct Renderer;
		friend struct Mesh;
		friend struct Model;
		friend struct Material;
		friend struct EnvironmentCubeMap;
		friend struct BasicMaterial;
		friend struct PBRMaterial;

		unsigned int _id;
		std::string _type;
	};
}