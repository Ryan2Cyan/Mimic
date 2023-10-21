#pragma once
#include <vector>
#include <string>
#include <lowlevelsystems/Mimic.h>
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
	struct Texture
	{
		unsigned int Id;
		std::string Type;
	};

	// #############################################################################
	// mesh stuct:
	// #############################################################################
	struct Shader;

	struct Mesh
	{
		explicit Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

		void Draw(std::shared_ptr<Shader> shader);

	private:
		friend struct Renderer;

		std::shared_ptr<Shader> _shader;
		std::vector<Vertex> _vertices;
		std::vector<unsigned int> _indices;
		std::vector<Texture> _textures;
		unsigned int _vertexArrayId;
		unsigned int _vertexBufferId;
		unsigned int _elementBufferId;
	};
}