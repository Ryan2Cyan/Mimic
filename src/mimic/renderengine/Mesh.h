#pragma once
#include <lowlevelsystems/Resources.h>
#include <vector>
#include <string>
#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <memory>

namespace Mimic
{
	// #############################################################################
	// vertex stuct:
	// #############################################################################
	struct Vertex
	{
		explicit Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 textureCoordinates);

		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TextureCoordinates;
	};

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

	// #############################################################################
	// mesh stuct:
	// #############################################################################
	struct Shader;
	struct Model;

	struct Mesh : Resource
	{
		explicit Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

		void Draw(std::shared_ptr<Shader> shader);

	private:
		friend struct ModelRenderer;

		std::vector<Vertex> _vertices;
		std::vector<unsigned int> _indices;
		std::vector<Texture> _textures;
		unsigned int _vertexArrayId;
		unsigned int _vertexBufferId;
		unsigned int _elementBufferId;
	};
}