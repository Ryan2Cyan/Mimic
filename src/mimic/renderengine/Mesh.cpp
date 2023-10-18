#include "Mesh.h"

namespace Mimic
{
	// #############################################################################
	// vertex defintions:
	// #############################################################################
	Vertex::Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 textureCoordinates) 
		: Position(position), Normal(normal), TextureCoordinates(textureCoordinates) { }

	// #############################################################################
	// mesh defintions:
	// #############################################################################
	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
		: Vertices(vertices), Indices(indices), Textures(textures) 
	{
		size_t sizeOfVertex = sizeof(Vertex);

		// generate & bind vertex array object:
		glGenVertexArrays(1, &_vertexArrayId);
		glBindVertexArray(_vertexArrayId);

		// generate & bind vertex position buffer object:
		glGenBuffers(1, &_vertexBufferId);
		glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferId);
		glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeOfVertex, &Vertices[0], GL_STATIC_DRAW);

		// generate & bind element buffer object (indices):
		glGenBuffers(1, &_elementBufferId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBufferId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), &Indices[0], GL_STATIC_DRAW);

		// pass buffer data into GPU - specify how openGL should read the buffers:
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeOfVertex, (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeOfVertex, (void*)offsetof(Vertex, Normal));
		glEnableVertexAttribArray(2);
	/*	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeOfVertex, (void*)offsetof(Vertex, TextureCoordinates));
		glDisableVertexAttribArray(0);*/

		glBindVertexArray(0);
	}

	void Mesh::Draw()
	{
		unsigned int diffuseN = 1;
		unsigned int specularN = 1;
		unsigned int length = Textures.size();
		for (unsigned int i = 0; i < length; i++)
		{
			// activate texture unit before binding:
			glActiveTexture(GL_TEXTURE0 + i);
			std::string textureNumber;
			std::string textureName = Textures[i].Type;
			if (textureName == "texture_diffuse") textureNumber = std::to_string(diffuseN++);
			else if(textureName == "texture_specular") textureNumber = std::to_string(specularN++);
		}
		glActiveTexture(GL_TEXTURE0);

		// draw mesh:
		glBindVertexArray(_vertexArrayId);
		glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}