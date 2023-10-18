#pragma once
#include <vector>
#include <string>
#include <GL/glew.h>
#include <GLM/glm.hpp>

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
	struct Texture
	{
		unsigned int Id;
		std::string Type;
	};

	// #############################################################################
	// mesh stuct:
	// #############################################################################
	struct Mesh
	{
		explicit Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

		std::vector<Vertex> Vertices;
		std::vector<unsigned int> Indices;
		std::vector<Texture> Textures;

		void Draw();

	private:
		unsigned int _vertexArrayId;
		unsigned int _vertexBufferId;
		unsigned int _elementBufferId;
	};
}