#pragma once
#include <lowlevelsystems/Resource.h>
#include <renderengine/Texture.h>
#include <renderengine/Vertex.h>
#include <vector>
#include <string>
#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <memory>

namespace Mimic
{
	// #############################################################################
	// mesh stuct:
	// #############################################################################
	struct Shader;
	struct Model;

	struct Mesh
	{
		explicit Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture>> textures);

		void Draw(std::shared_ptr<Shader> shader);

	private:
		friend struct ModelRenderer;

		std::vector<Vertex> _vertices;
		std::vector<unsigned int> _indices;
		std::vector<std::shared_ptr<Texture>> _textures;
		unsigned int _vertexArrayId;
		unsigned int _vertexBufferId;
		unsigned int _elementBufferId;
	};
}