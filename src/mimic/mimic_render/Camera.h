#pragma once
#include <GLM/glm.hpp>
#include <memory>

// Source: https://learnopengl.com/Getting-started/Camera

namespace MimicRender
{
	// #############################################################################
	// camera stuct:
	// #############################################################################
	struct Camera
	{
		static std::shared_ptr<Camera> Initialise(const glm::vec2& aspectRatio, const float& fov, const glm::vec2& clippingPlane = glm::vec2(0.1f, 100.0f));

		/// <summary> Re-calculates the camera's view and projection matrices based on it's current 
		/// orientation and position. </summary>
		void Update();

		glm::mat4 GetProjectionMatrix() const;
		glm::mat4 GetViewMatrix() const;

		glm::vec3 Position;
		glm::vec3 Orientation;
		glm::vec2 AspectRatio;
		glm::vec2 ClippingPlane;
		float Fov;
		
	private:
		friend struct Renderer;
		glm::vec3 _up;
		glm::mat4 _projectionMatrix;
		glm::mat4 _viewMatrix;
	};
}