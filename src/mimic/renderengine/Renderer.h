#pragma once
#include <GLM/glm.hpp>
#include <renderengine/Shader.h>
#include <vector>
#include <memory>


namespace Mimic
{
	// #############################################################################
	// render object class:
	// #############################################################################
	struct Texture;

	class RenderObject
	{
		friend struct ModelRenderer;
		friend struct Renderer;

		RenderObject(const unsigned int& vaoId, std::vector<std::shared_ptr<Texture>>& textures, std::vector<unsigned int>& indices, const std::shared_ptr<Shader>& shader, const glm::mat4& modelMatrix) noexcept;

		glm::mat4 _modelMatrix;
		std::vector<std::shared_ptr<Texture>> _textures;
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

		static std::shared_ptr<Renderer> Initialise();
		void AddToDrawQue(const RenderObject& renderObject);
		void Draw();

		glm::mat4 _cachedViewMatrix;
		glm::mat4 _cachedProjectionMatrix;
		std::vector<RenderObject> _renderQue;
	};
}