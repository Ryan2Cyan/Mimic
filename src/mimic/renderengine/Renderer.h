#pragma once
#include <GLM/glm.hpp>
#include <vector>
#include <functional>
#include <memory>
#include <array>


namespace Mimic
{
	// #############################################################################
	// render object class:
	// #############################################################################
	struct Texture;
	struct Shader;
	struct RenderTexture;

	class RenderObject
	{
		friend struct ModelRenderer;
		friend struct Renderer;

		RenderObject(const unsigned int& vaoId, std::vector<unsigned int>& indices, const std::shared_ptr<Shader>& shader, 
			         const glm::mat4& modelMatrix, std::function<void()>& onDrawLambda);

		glm::mat4 _modelMatrix;
		std::vector<std::shared_ptr<Texture>> _textures;
		std::vector<unsigned int> _indices;
		std::function<void()> _materialOnDraw;
		std::shared_ptr<Shader> _shader;
		unsigned int _vertexArrayId;
	
	};

	// #############################################################################
	// renderer stuct:
	// #############################################################################
	struct Renderer 
	{
	private:
		friend struct MimicCore;
		friend struct ModelRenderer;
		friend struct EnvironmentCubeMap;

		static std::shared_ptr<Renderer> Initialise();
		void AddToDrawQue(const RenderObject& renderObject);
		void Draw(const glm::vec3& viewPosition, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);

		void CaptureCubeMap(std::function<void()>& onDrawLambda, const std::shared_ptr<Shader>& shader, std::shared_ptr<RenderTexture>& renderTexture, const glm::ivec2& aspectRatio);
		/*void CapturePrefilteredCubeMap(std::function<void()>& onDrawLambda, const std::shared_ptr<Shader>& shader, std::shared_ptr<RenderTexture>& renderTexture, const glm::ivec2& aspectRatio, const unsigned int& mipLevels);*/
		void DrawUnitCube() noexcept;
		void DrawUnitQuad() noexcept;


		std::array<glm::mat4, 6> _captureViews;
		glm::mat4 _captureProjection;

		std::vector<RenderObject> _renderQue;

		unsigned int _unitQuadVertexArrayId;
		unsigned int _unitCubeVertexArrayId;
	};
}