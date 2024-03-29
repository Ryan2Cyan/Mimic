#pragma once
#include <GLM/glm.hpp>
#include <functional>
#include <memory>

namespace MimicRender
{
	struct Texture;
	struct Shader;
	struct RenderTexture;
	struct Camera;
	struct EnvironmentCubeMap;

	/// <summary>
	/// Specifically used by the Renderer struct, containing all the data needed to render an object.
	/// </summary>
	struct RenderObject
	{
		template<typename T>
		static std::shared_ptr<RenderObject> Initialise(const unsigned int& vaoId, const unsigned int& dataSize, const std::shared_ptr<Shader>& shader,
			const glm::mat4& modelMatrix, T& onDrawLambda)
		{
			std::shared_ptr<RenderObject> renderObject = std::make_shared<RenderObject>();
			renderObject->_vertexArrayId = vaoId;
			renderObject->_dataSize = dataSize;
			renderObject->_shader = shader;
			renderObject->_modelMatrix = modelMatrix;
			renderObject->_onDraw = onDrawLambda;

			return renderObject;
		}

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

	/// <summary>
	/// Contains a queue of RenderObjects, when Renderer::Draw() is called all RenderObjects are rendered via the
	/// GPU.
	/// </summary>
	struct Renderer 
	{
		static std::shared_ptr<Renderer> Initialise();

		/// <summary>
		/// Push back render object to draw queue. Objects in the draw queue will be drawn when
		/// Renderer::Draw() is called in any context. Ensure Renderer::ClearRenderQueue() is the called
		/// before the next render loop.
		/// </summary>
		void AddToDrawQueue(const std::shared_ptr<RenderObject>& renderObject);

		/// <summary>
		/// Loop over all render objects in render queue, drawing each to the window.
		/// </summary>
		void Draw(const std::shared_ptr<Camera>& camera);

		/// <summary>
		/// Loop over all render objects in render queue, drawing each to the window.
		/// </summary>
		void Draw(const glm::mat4& view, const glm::mat4& projection);

		/// <summary>
		/// Draws environment map to the window. Ensure scene objects are rendered first.
		/// </summary>
		void DrawEnvironmentMap(const std::shared_ptr<Camera>& camera, const std::shared_ptr<EnvironmentCubeMap>& environmentCubeMap);

		/// <summary>
		/// Clears all render objects in render queue. Ensure this is called before entering the
		/// next render loop.
		/// </summary>
		void ClearRenderQueue() noexcept;

	private:
		friend struct EnvironmentCubeMap;

		void DrawUnitCube() noexcept;
		void DrawUnitQuad() noexcept;
		
		render_object_vector _renderQueue;
		unsigned int _unitQuadVertexArrayId;
		unsigned int _unitCubeVertexArrayId;
	};
}