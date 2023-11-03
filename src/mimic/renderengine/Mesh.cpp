#include "Mesh.h"
#include <utility/Logger.h>
#include <GL/glew.h>

namespace Mimic
{

	// #############################################################################
	// mesh defintions:
	// #############################################################################
	// Source: https://learnopengl.com/Model-Loading/Mesh

	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) : _vertices(vertices), _indices(indices) 
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

	void Mesh::SetDiffuse(const std::shared_ptr<Texture>& diffuse)
	{
		if (diffuse == nullptr) 
		{ 
			MIMIC_LOG_WARNING("[Mesh] Unable to assign diffuse texture.");
			return;
		}
		_diffuseTexture = diffuse;
	}

	void Mesh::SetSpecular(const std::shared_ptr<Texture>& specular)
	{
		if (specular == nullptr)
		{
			MIMIC_LOG_WARNING("[Mesh] Unable to assign specular texture.");
			return;
		}
		_specularTexture = specular;
	}

	void Mesh::SetNormal(const std::shared_ptr<Texture>& normal)
	{
		if (normal == nullptr)
		{
			MIMIC_LOG_WARNING("[Mesh] Unable to assign normal texture.");
			return;
		}
		_normalTexture = normal;
	}

	void Mesh::SetHeight(const std::shared_ptr<Texture>& height)
	{
		if (height == nullptr)
		{
			MIMIC_LOG_WARNING("[Mesh] Unable to assign height texture.");
			return;
		}
		_heightTexture = height;
	}

}