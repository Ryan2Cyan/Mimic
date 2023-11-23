#include "Renderer.h"
#include <utility/Logger.h>
#include <renderengine/Shader.h>
#include <renderengine/RenderTexture.h>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <GL/glew.h>

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
		std::shared_ptr<Renderer> renderer = std::make_shared<Renderer>();
		renderer->_captureProjection = glm::mat4(1.0f);
		renderer->_captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
		renderer->_captureViews =
		{
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
		};
		
		return renderer;
	}

	void Renderer::AddToDrawQue(const RenderObject& renderObject)
	{
		_renderQue.push_back(renderObject);
	}

	void Renderer::Draw(const glm::vec3& viewPosition, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
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

			// draw mesh:
			glBindVertexArray(renderObject._vertexArrayId);
			glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(renderObject._indices.size()), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
			glUseProgram(0);
		}
	}

	void Renderer::CaptureCubeMap(std::function<void()>& onDrawLambda, const std::shared_ptr<Shader>& shader, std::shared_ptr<RenderTexture>& renderTexture, const glm::ivec2& aspectRatio)
	{
		 renderTexture->UseRenderObject(aspectRatio);
		 shader->UseShader();
		 shader->SetMat4("u_Projection", _captureProjection);
		 onDrawLambda();

		 constexpr int startTargetIndex = (int)TextureTarget::MIMIC_CUBE_MAP_POSITIVE_X;
		 for (unsigned int i = 0; i < 6; i++)
		 {
			 shader->SetMat4("u_View", _captureViews[i]);
			 renderTexture->BindTextureForRender((TextureTarget)(startTargetIndex + i));

			 // render unit cube:
			 DrawUnitCube();
		 }
		 renderTexture->Unbind();
	}

	//void Renderer::CapturePrefilteredCubeMap(std::function<void()>& onDrawLambda, const std::shared_ptr<Shader>& shader, std::shared_ptr<RenderTexture>& renderTexture, const glm::ivec2& aspectRatio, const unsigned int& mipLevels)
	//{
	//	shader->UseShader();
	//	shader->SetMat4("u_Projection", _captureProjection);
	//	onDrawLambda();

	//	constexpr int startTargetIndex = (int)TextureTarget::MIMIC_CUBE_MAP_POSITIVE_X;
	//	for (unsigned int mip = 0; mip < mipLevels; mip++)
	//	{
	//		// resize framebuffer according to mip-level size:
	//		const unsigned int mipWidth = aspectRatio.x * std::pow(0.5f, mip);
	//		const unsigned int mipHeight = aspectRatio.y * std::pow(0.5f, mip);;
	//		renderTexture->UseRenderObject(glm::ivec2(mipWidth, mipHeight));
	//		float roughness = (float)mip / (float)(mipLevels - 1);

	//		for (unsigned int i = 0; i < 6; i++)
	//		{
	//			shader->SetMat4("u_View", _captureViews[i]);
	//			renderTexture->BindTextureForRender((TextureTarget)(startTargetIndex + i));

	//			// render unit cube:
	//			DrawUnitCube();
	//		}
	//	}
	//	renderTexture->Unbind();
	//}

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

}