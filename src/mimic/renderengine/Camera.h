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
		static const std::shared_ptr<Camera> Initialise(const glm::vec2& aspectRatio, const float& fov);
		void Update();

		glm::vec3 Position;
		glm::vec3 Right;
		glm::vec3 Up;
		glm::vec3 Direction; // needs to be reversed.
		glm::vec3 Orientation; // needs to be reversed.
		glm::vec3 Target;
		glm::vec2 AspectRatio;
		glm::vec2 ClippingPlane;
		float Fov;
		
	private:
		glm::mat4 _projectionMatrix;
		glm::mat4 _viewMatrix;
	};
}