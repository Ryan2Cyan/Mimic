#include "Renderer.h"
#include <renderengine/Shader.h>

namespace Mimic
{
	// #############################################################################
	// render object member functions:
	// #############################################################################
	RenderObject::RenderObject(const unsigned int& vaoId, std::vector<unsigned int>& indices, const std::shared_ptr<Shader>& shader, const glm::mat4& modelMatrix, 
		                        std::function<void()>& materialOnDraw)
								: _vertexArrayId(vaoId), _indices(indices), _shader(shader), _modelMatrix(modelMatrix),
								_materialOnDraw(materialOnDraw) { }

	// #############################################################################
	// renderer functions:
	// #############################################################################
	std::shared_ptr<Renderer> Renderer::Initialise()
	{
		return std::make_shared<Renderer>();
	}

	void Renderer::AddToDrawQue(const RenderObject& renderObject)
	{
		_renderQue.push_back(renderObject);
	}

	void Renderer::Draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
	{
		for (RenderObject renderObject : _renderQue)
		{
			// set uniforms:
			glUseProgram(renderObject._shader->_shaderProgramId);
			renderObject._shader->SetModelMatrix(renderObject._modelMatrix);
			renderObject._shader->SetViewMatrix(viewMatrix);
			renderObject._shader->SetProjectionMatrix(projectionMatrix);
			renderObject._materialOnDraw();
			glActiveTexture(GL_TEXTURE0);

			// draw mesh:
			glBindVertexArray(renderObject._vertexArrayId);
			glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(renderObject._indices.size()), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
			glUseProgram(0);
		}
	}
}