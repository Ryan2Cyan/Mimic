#pragma once
#include <GLM/glm.hpp>
#include <vector>
#include <functional>
#include <memory>
#include <array>


namespace MimicRender
{
	// #############################################################################
	// render object class:
	// #############################################################################
	struct Texture;
	struct Shader;
	struct RenderTexture;
	struct Camera;

	struct RenderObject
	{
		static const std::shared_ptr<RenderObject> Initialise(const unsigned int& vaoId, const unsigned int& dataSize, const std::shared_ptr<Shader>& shader,
			const glm::mat4& modelMatrix, std::function<void()>& onDrawLambda);

	private:
		friend struct Renderer;

		glm::mat4 _modelMatrix;
		std::vector<std::shared_ptr<Texture>> _textures;
		unsigned int _dataSize;
		std::function<void()> _onDraw;
		std::shared_ptr<Shader> _shader;
		unsigned int _vertexArrayId;
	
	};

	typedef std::vector<std::shared_ptr<RenderObject>> render_object_vector;

	// #############################################################################
	// renderer stuct:
	// #############################################################################
	struct Renderer 
	{
		static std::shared_ptr<Renderer> Initialise();
		void AddToDrawQue(const std::shared_ptr<RenderObject>& renderObject);
		void Draw(const std::shared_ptr<Camera>& camera);

	private:

		void CaptureCubeMap(std::function<void()>& onDrawLambda, const std::shared_ptr<Shader>& shader, std::shared_ptr<RenderTexture>& renderTexture, const glm::ivec2& aspectRatio);
		/*void CapturePrefilteredCubeMap(std::function<void()>& onDrawLambda, const std::shared_ptr<Shader>& shader, std::shared_ptr<RenderTexture>& renderTexture, const glm::ivec2& aspectRatio, const unsigned int& mipLevels);*/
		void DrawUnitCube() noexcept;
		void DrawUnitQuad() noexcept;


		std::array<glm::mat4, 6> _captureViews;
		glm::mat4 _captureProjection;

		render_object_vector _renderQue;

		unsigned int _unitQuadVertexArrayId;
		unsigned int _unitCubeVertexArrayId;
	};
}