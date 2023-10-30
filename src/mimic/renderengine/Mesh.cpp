#include "Mesh.h"
#include <renderengine/Shader.h>
#include <renderengine/Vertex.h>
#include <renderengine/Texture.h>

namespace Mimic
{

	// #############################################################################
	// mesh defintions:
	// #############################################################################
	// Source: https://learnopengl.com/Model-Loading/Mesh

	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture>> textures)
		: _vertices(vertices), _indices(indices), _textures(textures) 
	{
		constexpr size_t VERTEX_SIZE = sizeof(Vertex);

		// generate & bind vertex array object:
		glGenVertexArrays(1, &_vertexArrayId);
		glBindVertexArray(_vertexArrayId);

		// generate & bind vertex position buffer object:
		glGenBuffers(1, &_vertexBufferId);
		glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferId);
		glBufferData(GL_ARRAY_BUFFER, _vertices.size() * VERTEX_SIZE, &_vertices[0], GL_STATIC_DRAW);

		// generate & bind element buffer object (indices):
		glGenBuffers(1, &_elementBufferId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBufferId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), &_indices[0], GL_STATIC_DRAW);

		// pass buffer data into GPU - specify how openGL should read the buffers:
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (void*)offsetof(Vertex, Normal));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (void*)offsetof(Vertex, TextureCoordinates));

		glBindVertexArray(0);
	}

	void Mesh::Draw(std::shared_ptr<Shader> shader)
	{
		unsigned int textureIndex = 1;

		const unsigned int length = _textures.size();
		shader->SetInt("materialsCount", length);

		for (unsigned int i = 0; i < length; i++)
		{
			// activate texture unit before binding:
			glActiveTexture(GL_TEXTURE0 + i);
			std::string textureNumber = std::to_string(i);
			// will probably need to insert the type eventually:
			shader->SetInt(("materials[" + textureNumber + "].texture").c_str(), i);
			glBindTexture(GL_TEXTURE_2D, _textures[i]->Id);
		}
		glActiveTexture(GL_TEXTURE0);

		// draw mesh:
		glBindVertexArray(_vertexArrayId);
		glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(_indices.size()), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}