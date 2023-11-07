#include "Renderer.h"
#include <renderengine/Shader.h>
#include <renderengine/Light.h>
#include <string>

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

	void Renderer::Draw(const glm::vec4& viewPosition, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const std::vector<std::shared_ptr<DirectLight>>& lights)
	{
		for (RenderObject renderObject : _renderQue)
		{
			// set uniforms:
			glUseProgram(renderObject._shader->_shaderProgramId);
			renderObject._shader->SetVector4("u_ViewPosition", viewPosition);
			renderObject._shader->SetModelMatrix(renderObject._modelMatrix);
			renderObject._shader->SetViewMatrix(viewMatrix);
			renderObject._shader->SetProjectionMatrix(projectionMatrix);
			for (int i = 0; i < lights.size(); i++)
			{
				const std::string currentLightName = "u_DirectLights[" + std::to_string(i) + "]";

				const glm::vec4 position = glm::vec4(lights[i]->Position.x, lights[i]->Position.y, lights[i]->Position.z, 1.0f);
				renderObject._shader->SetVector4((currentLightName + ".position").c_str(), position);

				// const glm::vec4 direction = glm::vec4(lights[i]->Direction.x, lights[i]->Direction.y, lights[i]->Direction.z, 0.0f);
				// renderObject._shader->SetVector4((currentLightName + ".direction").c_str(), direction);

				const glm::vec4 colour = glm::vec4(lights[i]->Colour.x, lights[i]->Colour.y, lights[i]->Colour.z, 1.0f);
				renderObject._shader->SetVector4((currentLightName + ".colour").c_str(), colour);
			}
			renderObject._shader->SetInt("u_DirectLightsCount", lights.size());
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