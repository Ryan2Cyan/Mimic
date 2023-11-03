#include "Mesh.h"
#include <GL/glew.h>

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
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (void*)offsetof(Vertex, _normal));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (void*)offsetof(Vertex, _textureCoordinates));

		glBindVertexArray(0);
	}
}