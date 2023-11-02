#pragma once
#include <renderengine/Shader.h>
#include <renderengine/Texture.h>
#include <GLM/glm.hpp>
#include <vector>
#include <memory>


namespace Mimic
{
	// #############################################################################
	// render object class:
	// #############################################################################
	class RenderObject
	{
		friend struct ModelRenderer;
		friend struct Renderer;

		RenderObject(const unsigned int& vaoId, std::vector<Texture>& textures, std::vector<unsigned int>& indices, const std::shared_ptr<Shader>& shader) noexcept;

		glm::mat4 _modelMatrix;
		std::vector<Texture> _textures;
		std::vector<unsigned int> _indices;
		std::shared_ptr<Shader> _shader;
		unsigned int _vertexArrayId;
		// material
	};

	// #############################################################################
	// renderer stuct:
	// #############################################################################
	struct Renderer 
	{
	private:
		friend struct MimicCore;
		friend struct ModelRenderer;

		std::shared_ptr<Renderer> Initialise();
		void AddToDrawQue(const RenderObject& renderObject);
		void Draw();

		glm::mat4 _viewMatrix;
		glm::mat4 _projectionMatrix;
		std::vector<RenderObject> _renderQue;
	};
}