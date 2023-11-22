#include "Renderer.h"
#include <utility/Logger.h>
#include <renderengine/Shader.h>
#include <renderengine/Light.h>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include <string>

namespace Mimic
{
	// #############################################################################
	// render object member functions:
	// #############################################################################
	RenderObject::RenderObject(const unsigned int& vaoId, std::vector<unsigned int>& indices, const std::shared_ptr<Shader>& shader, const glm::mat4& modelMatrix, 
		                        std::function<void()>& onDrawLambda)
								: _vertexArrayId(vaoId), _indices(indices), _shader(shader), _modelMatrix(modelMatrix),
								_materialOnDraw(onDrawLambda) { }

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

	void Renderer::Draw(const glm::vec3& viewPosition, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const std::vector<std::shared_ptr<DirectLight>>& directLights, const std::vector<std::shared_ptr<PointLight>>& pointLights)
	{
		for (RenderObject renderObject : _renderQue)
		{

			// set uniforms:
			renderObject._shader->UseShader();
			renderObject._shader->SetVector3("u_CameraPosition", viewPosition);
			renderObject._shader->SetModelMatrix(renderObject._modelMatrix);
			renderObject._shader->SetViewMatrix(viewMatrix);
			renderObject._shader->SetProjectionMatrix(projectionMatrix);
			renderObject._materialOnDraw();

			// direct lights:
			for (int i = 0; i < directLights.size(); i++)
			{
				const std::string currentLight = "u_DirectLights[" + std::to_string(i) + "]";

				// renderObject._shader->SetVector3((currentLight + ".position").c_str(), directLights[i]->Position);
				// negate the direction of the light to be coming from the light source
				renderObject._shader->SetVector3((currentLight + ".direction").c_str(), glm::normalize(-directLights[i]->Direction));
				const glm::vec4 colour = glm::vec4(directLights[i]->Colour.x, directLights[i]->Colour.y, directLights[i]->Colour.z, 1.0f);
				renderObject._shader->SetVector4((currentLight + ".colour").c_str(), colour);
			}
			renderObject._shader->SetInt("u_DirectLightsCount", directLights.size());


			// direct lights:
			for (int i = 0; i < pointLights.size(); i++)
			{
				const std::string currentLight = "u_PointLights[" + std::to_string(i) + "]";

				renderObject._shader->SetVector3((currentLight + ".position").c_str(), pointLights[i]->Position);
				const glm::vec4 colour = glm::vec4(pointLights[i]->Colour.x, pointLights[i]->Colour.y, pointLights[i]->Colour.z, 1.0f);
				renderObject._shader->SetVector4((currentLight + ".colour").c_str(), colour);
				renderObject._shader->SetFloat((currentLight + ".constant").c_str(), pointLights[i]->Constant);
				renderObject._shader->SetFloat((currentLight + ".linear").c_str(), pointLights[i]->Linear);
				renderObject._shader->SetFloat((currentLight + ".quadratic").c_str(), pointLights[i]->Quadratic);
			}
			renderObject._shader->SetInt("u_PointLightsCount", pointLights.size());

			// draw mesh:
			glBindVertexArray(renderObject._vertexArrayId);
			glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(renderObject._indices.size()), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
			glUseProgram(0);
		}
	}
}