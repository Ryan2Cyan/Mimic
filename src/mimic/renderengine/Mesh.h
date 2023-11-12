#pragma once
#include <renderengine/Texture.h>
#include <renderengine/Vertex.h>
#include <vector>
#include <GLM/glm.hpp>
#include <memory>

namespace Mimic
{
	// #############################################################################
	// mesh stuct:
	// #############################################################################
	struct Model;

	struct Mesh
	{
		explicit Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
	private:
		friend struct ModelRenderer;
		friend struct Model;
		friend struct CubeMap;

		void SetDiffuse(const std::shared_ptr<Texture>& diffuse);
		void SetSpecular(const std::shared_ptr<Texture>& specular);
		void SetNormal(const std::shared_ptr<Texture>& normal);
		void SetHeight(const std::shared_ptr<Texture>& height);

		std::vector<Vertex> _vertices;
		std::vector<unsigned int> _indices;
		std::vector<std::shared_ptr<Texture>> _textures;
		std::shared_ptr<Texture> _diffuseTexture;
		std::shared_ptr<Texture> _specularTexture;
		std::shared_ptr<Texture> _normalTexture;
		std::shared_ptr<Texture> _heightTexture;
		unsigned int _vertexArrayId;
		unsigned int _vertexBufferId;
		unsigned int _elementBufferId;
	};
}