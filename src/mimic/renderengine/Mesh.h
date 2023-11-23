#pragma once
#include <renderengine/Texture.h>
#include <renderengine/Vertex.h>
#include <vector>
#include <GLM/glm.hpp>
#include <memory>

namespace MimicRender
{
	// #############################################################################
	// mesh stuct:
	// #############################################################################
	struct Model;
	struct Vertex;

	typedef std::vector<std::shared_ptr<Vertex>> vertex_vector;

	struct Mesh
	{
		static const std::shared_ptr<Mesh> Initialise(const vertex_vector& vertices, const std::vector<unsigned int>& indices);
		const unsigned int GetVertexArrayId() const noexcept;
	private:
		/*friend struct ModelRenderer;
		friend struct Model;
		friend struct CubeMap;
		friend struct EnvironmentCubeMap;*/

	/*	void SetDiffuse(const std::shared_ptr<Texture>& diffuse);
		void SetSpecular(const std::shared_ptr<Texture>& specular);
		void SetNormal(const std::shared_ptr<Texture>& normal);
		void SetHeight(const std::shared_ptr<Texture>& height);*/

	/*	std::vector<std::shared_ptr<Texture>> _textures;
		std::shared_ptr<Texture> _diffuseTexture;
		std::shared_ptr<Texture> _specularTexture;
		std::shared_ptr<Texture> _normalTexture;
		std::shared_ptr<Texture> _heightTexture;*/

		unsigned int _vertexArrayId = 0;
		bool _intialised = false;
	};
}