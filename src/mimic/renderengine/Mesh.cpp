#include "Mesh.h"

namespace Mimic
{
	// #############################################################################
	// vertex defintions:
	// #############################################################################
	Vertex::Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 textureCoordinates) 
		: Position(position), Normal(normal), TextureCoordinates(textureCoordinates) { }


	// #############################################################################
	// vertex defintions:
	// #############################################################################
	Texture::Texture(const unsigned int id, std::string type, std::string path) : Id(id), Type(type), Path(path) {}


	// #############################################################################
	// mesh defintions:
	// #############################################################################
	// Source: https://learnopengl.com/Model-Loading/Mesh

	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
		: _vertices(vertices), _indices(indices), _textures(textures) 
	{
		constexpr size_t sizeOfVertex = sizeof(Vertex);

		// generate & bind vertex array object:
		glGenVertexArrays(1, &_vertexArrayId);
		glBindVertexArray(_vertexArrayId);

		// generate & bind vertex position buffer object:
		glGenBuffers(1, &_vertexBufferId);
		glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferId);
		glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeOfVertex, &_vertices[0], GL_STATIC_DRAW);

		// generate & bind element buffer object (indices):
		glGenBuffers(1, &_elementBufferId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBufferId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), &_indices[0], GL_STATIC_DRAW);

		// pass buffer data into GPU - specify how openGL should read the buffers:
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeOfVertex, (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeOfVertex, (void*)offsetof(Vertex, Normal));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeOfVertex, (void*)offsetof(Vertex, TextureCoordinates));

		glBindVertexArray(0);
	}

	void Mesh::Draw(std::shared_ptr<Shader> shader)
	{
		unsigned int diffuseN = 1;
		unsigned int specularN = 1;
		unsigned int normalN = 1;
		unsigned int heightN = 1;

		const unsigned int length = _textures.size();
		for (unsigned int i = 0; i < length; i++)
		{
			// activate texture unit before binding:
			glActiveTexture(GL_TEXTURE0 + i);
			std::string textureNumber;
			std::string textureName = _textures[i].Type;

			if (textureName == "texture_diffuse") textureNumber = std::to_string(diffuseN++);
			else if(textureName == "texture_specular") textureNumber = std::to_string(specularN++);
			else if(textureName == "texture_normal")  textureNumber = std::to_string(normalN++);
			else if (textureName == "texture_height")  textureNumber = std::to_string(heightN++);

			// Set sampler to correct texture unit:
			/*shader->SetInt((textureName + textureNumber).c_str(), i);
			glBindTexture(GL_TEXTURE_2D, _textures[i].Id);*/
		}

		// draw mesh:
		glBindVertexArray(_vertexArrayId);
		glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(_indices.size()), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glActiveTexture(GL_TEXTURE0);
	}
}