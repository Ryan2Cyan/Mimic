#include "Mesh.h"
#include <mimic_render/Vertex.h>
#include <mimic_utility/Logger.h>

#include <GL/glew.h>

namespace MimicRender
{

	// #############################################################################
	// mesh defintions:
	// #############################################################################
	// Source: https://learnopengl.com/Model-Loading/Mesh

	std::shared_ptr<Mesh> Mesh::Initialise(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
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
		mesh->_vertices = vertices;
		mesh->_indices = indices;

		std::vector<Triangle> triangles;
		for (size_t i = 0; i < indices.size(); i += 3)
		{
			triangles.push_back(Triangle(
				vertices[indices[i]].Position,
				vertices[indices[i + 1]].Position,
				vertices[indices[i + 2]].Position
			));
		}
		mesh->_triangles = triangles;
		return mesh;
	}

	unsigned int Mesh::GetVertexArrayId() const noexcept
	{
		if (!_intialised)
		{
			MIMIC_LOG_WARNING("[MimicRender::Mesh] Attempting to access Mesh that is uninitialised.");
			return 0;
		}
		return _vertexArrayId;
	}

	unsigned int Mesh::GetDataSize() const noexcept
	{
		if (!_intialised)
		{
			MIMIC_LOG_WARNING("[MimicRender::Mesh] Attempting to access Mesh that is uninitialised.");
			return 0;
		}
		return _dataSize;
	}

	std::vector<Vertex> Mesh::GetVertices() const noexcept
	{
		if (!_intialised)
		{
			MIMIC_LOG_WARNING("[MimicRender::Mesh] Attempting to access Mesh that is uninitialised.");
			return { };
		}
		return _vertices;
	}

	std::vector<unsigned int> Mesh::GetIndices() const noexcept
	{
		if (!_intialised)
		{
			MIMIC_LOG_WARNING("[MimicRender::Mesh] Attempting to access Mesh that is uninitialised.");
			return { };
		}
		return _indices;
	}

	std::vector<Triangle> Mesh::GetTriangles() const noexcept
	{
		if (!_intialised)
		{
			MIMIC_LOG_WARNING("[MimicRender::Mesh] Attempting to access Mesh that is uninitialised.");
			return { };
		}
		return _triangles;
	}
}