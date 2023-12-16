#pragma once
#include <vector>
#include <memory>

namespace MimicRender
{
	// #############################################################################
	// mesh stuct:
	// #############################################################################
	struct Model;
	struct Vertex;

	typedef std::vector<Vertex> vertex_vector;

	struct Mesh
	{
		static std::shared_ptr<Mesh> Initialise(const vertex_vector& vertices, const std::vector<unsigned int>& indices);
		unsigned int GetVertexArrayId() const noexcept;
		unsigned int GetDataSize() const noexcept;
	private:
		friend struct Model;

		unsigned int _vertexArrayId = 0;
		unsigned int _dataSize = 0;
		bool _intialised = false;
	};
}