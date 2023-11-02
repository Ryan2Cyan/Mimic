#include "Renderer.h"
#include <utility>
#include <GL/glew.h>

namespace Mimic
{
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

	void Renderer::Draw()
	{
		for (RenderObject renderObject : _renderQue)
		{
			glUseProgram(renderObject._shader->ShaderProgramId);

			renderObject._shader->SetModelMatrix(renderObject._modelMatrix);
			renderObject._shader->SetViewMatrix(_viewMatrix);
			renderObject._shader->SetProjectionMatrix(_projectionMatrix);

			unsigned int textureIndex = 1;

			const unsigned int length = renderObject._textures.size();
			renderObject._shader->SetInt("materialsCount", length);

			for (unsigned int i = 0; i < length; i++)
			{
				// activate texture unit before binding:
				glActiveTexture(GL_TEXTURE0 + i);
				std::string textureNumber = std::to_string(i);
				// will probably need to insert the type eventually:
				renderObject._shader->SetInt(("materials[" + textureNumber + "].texture").c_str(), i);
				glBindTexture(GL_TEXTURE_2D, renderObject._textures[i].Id);
			}
			glActiveTexture(GL_TEXTURE0);

			// draw mesh:
			glBindVertexArray(renderObject._vertexArrayId);
			glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(renderObject._indices.size()), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
			glUseProgram(0);
		}
		_renderQue.clear();
	}

	// #############################################################################
	// render object member functions:
	// #############################################################################
	RenderObject::RenderObject(const unsigned int& vaoId, std::vector<Texture>& textures, std::vector<unsigned int>& indices, const std::shared_ptr<Shader>& shader) noexcept
		: _vertexArrayId(vaoId), _textures(textures), _indices(indices), _shader(shader) {}
}