#include "Mesh.h"
#include <utility/Logger.h>
#include <GL/glew.h>

namespace MimicRender
{

	// #############################################################################
	// mesh defintions:
	// #############################################################################
	// Source: https://learnopengl.com/Model-Loading/Mesh

	const std::shared_ptr<Mesh> Mesh::Initialise(const vertex_vector& vertices, const std::vector<unsigned int>& indices)
	{
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
		constexpr size_t VERTEX_SIZE = sizeof(Vertex);

		// generate & bind vertex array object:
		glGenVertexArrays(1, &mesh->_vertexArrayId);
		glBindVertexArray(mesh->_vertexArrayId);

		// generate & bind vertex position buffer object (vertices):
		unsigned int vertexBufferId;
		glGenBuffers(1, &vertexBufferId);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * VERTEX_SIZE, &vertices[0], GL_STATIC_DRAW);

		// generate & bind element buffer object (indices):
		unsigned int elementBufferId;
		glGenBuffers(1, &elementBufferId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		// pass buffer data into GPU - specify how openGL should read the buffers:
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (void*)offsetof(Vertex, _normal));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (void*)offsetof(Vertex, _tangent));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (void*)offsetof(Vertex, _biTangent));
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (void*)offsetof(Vertex, _textureCoordinates));

		glBindVertexArray(0);
		mesh->_intialised = true;
		mesh->_dataSize = static_cast<unsigned int>(indices.size());

		return mesh;
	}

	const unsigned int Mesh::GetVertexArrayId() const noexcept
	{
		if (!_intialised) MIMIC_LOG_WARNING("[MimicRender::Mesh] Attempting to access Mesh that is uninitialised.");
		return _vertexArrayId;
	}

	/*void Mesh::SetDiffuse(const std::shared_ptr<Texture>& diffuse)
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
	}*/

}