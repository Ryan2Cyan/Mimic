#include "Renderer.h"
#include <renderengine/Shader.h>
#include <renderengine/RenderTexture.h>
#include <renderengine/Camera.h>
#include <renderengine/CubeMap.h>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

namespace MimicRender
{
	// #############################################################################
	// render object member functions:
	// #############################################################################

	const std::shared_ptr<RenderObject> RenderObject::Initialise(const unsigned int& vaoId, const unsigned int& dataSize, const std::shared_ptr<Shader>& shader,
		const glm::mat4& modelMatrix, std::function<void()>& onDrawLambda)
	{
		std::shared_ptr<RenderObject> renderObject = std::make_shared<RenderObject>();
		renderObject->_vertexArrayId = vaoId;
		renderObject->_dataSize = dataSize;
		renderObject->_shader = shader;
		renderObject->_modelMatrix = modelMatrix;
		renderObject->_onDraw = onDrawLambda;

		return renderObject;
	}

	// #############################################################################
	// renderer functions:
	// #############################################################################
	std::shared_ptr<Renderer> Renderer::Initialise()
	{
		return std::make_shared<Renderer>();
	}

	void Renderer::AddToDrawQue(const std::shared_ptr<RenderObject>& renderObject)
	{
		_renderQue.push_back(renderObject);
	}

	void Renderer::Draw(const std::shared_ptr<Camera>& camera)
	{
		for (std::shared_ptr<RenderObject> renderObject : _renderQue)
		{
			// set uniforms:
			renderObject->_shader->UseShader();
			renderObject->_onDraw();
			renderObject->_shader->SetVector3("u_CameraPosition", camera->Position);
			renderObject->_shader->SetModelMatrix(renderObject->_modelMatrix);
			renderObject->_shader->SetViewMatrix(camera->_viewMatrix);
			renderObject->_shader->SetProjectionMatrix(camera->_projectionMatrix);

			// draw mesh:
			glBindVertexArray(renderObject->_vertexArrayId);
			glDrawElements(GL_TRIANGLES, renderObject->_dataSize, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
			glUseProgram(0);
		}
	}

	void Renderer::Draw(const glm::mat4& view, const glm::mat4& projection)
	{
		for (std::shared_ptr<RenderObject> renderObject : _renderQue)
		{
			// set uniforms:
			renderObject->_shader->UseShader();
			//renderObject->_shader->SetVector3("u_CameraPosition", camera->Position);
			renderObject->_shader->SetModelMatrix(renderObject->_modelMatrix);
			renderObject->_shader->SetViewMatrix(view);
			renderObject->_shader->SetProjectionMatrix(projection);
			renderObject->_onDraw();

			// draw mesh:
			glBindVertexArray(renderObject->_vertexArrayId);
			glDrawElements(GL_TRIANGLES, renderObject->_dataSize, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
			glUseProgram(0);
		}
	}

	void Renderer::DrawCubeMap(const std::shared_ptr<Camera>& camera, const std::shared_ptr<EnvironmentCubeMap>& environmentCubeMap)
	{
		if (!environmentCubeMap->_initialised) return;

		glDepthFunc(GL_LEQUAL);
		environmentCubeMap->_cubeMapShader->UseShader();
		environmentCubeMap->_cubeMapShader->SetMat4("u_View", camera->_viewMatrix);
		environmentCubeMap->_cubeMapShader->SetMat4("u_Projection", camera->_projectionMatrix);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, environmentCubeMap->_environmentMapRenderTexture->_texture->_id);
		DrawUnitCube();
		glBindVertexArray(0);

		glDepthFunc(GL_LESS);
	}

	void Renderer::DrawUnitCube() noexcept
	{
		// initialise (if necessary):
		if (_unitCubeVertexArrayId == 0)
		{
			float vertices[] = {
				// back face:
				-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
				 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
				 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
				 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
				-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
				-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
				// front face:
				-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
				 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
				 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
				 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
				-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
				-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
				// left face:
				-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
				-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
				-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
				-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
				-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
				-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
				// right face:
				 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
				 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
				 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
				 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
				 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
				 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
				 // bottom face:
				 -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
				  1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
				  1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
				  1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
				 -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
				 -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
				 // top face:
				 -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
				  1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
				  1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
				  1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
				 -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
				 -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
			};
			glGenVertexArrays(1, &_unitCubeVertexArrayId);
			unsigned int cubeVBO;
			glGenBuffers(1, &cubeVBO);

			glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			glBindVertexArray(_unitCubeVertexArrayId);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}
		// draw unit cube:
		glBindVertexArray(_unitCubeVertexArrayId);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}

	void Renderer::DrawUnitQuad() noexcept
	{
		// initialise (if necessary):
		if (_unitQuadVertexArrayId == 0)
		{
			unsigned int quadVBO;
			float quadVertices[] = {
				// positions          // texture Coords
				-1.0f,  1.0f, 0.0f,   0.0f, 1.0f,
				-1.0f, -1.0f, 0.0f,   0.0f, 0.0f,
				 1.0f,  1.0f, 0.0f,   1.0f, 1.0f,
				 1.0f, -1.0f, 0.0f,   1.0f, 0.0f,
			};

			glGenVertexArrays(1, &_unitQuadVertexArrayId);
			glGenBuffers(1, &quadVBO);
			glBindVertexArray(_unitQuadVertexArrayId);
			glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		}
		glBindVertexArray(_unitQuadVertexArrayId);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}

	void Renderer::ClearRenderQue() noexcept
	{
		_renderQue.clear();
	}
}