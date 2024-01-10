#pragma once
#include "Triangle.h"

#include <vector>
#include <memory>

namespace MimicRender
{
	/// <summary>
	/// Contains vertices, indices, and triangles of a model mesh. See Model struct to load in meshes.
	/// </summary>
	struct Model;
	struct Vertex;

	struct Mesh
	{
		static std::shared_ptr<Mesh> Initialise(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
		unsigned int GetVertexArrayId() const noexcept;
		unsigned int GetDataSize() const noexcept;
		std::vector<Vertex> GetVertices() const noexcept;
		std::vector<unsigned int> GetIndices() const noexcept;
		std::vector<Triangle> GetTriangles() const noexcept;

	private:
		friend struct Model;
		std::vector<Triangle> _triangles;
		std::vector<Vertex> _vertices;
		std::vector<unsigned int> _indices;
		unsigned int _vertexArrayId = 0;
		unsigned int _dataSize = 0;
		bool _intialised = false;
	};
}