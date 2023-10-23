#pragma once
#include <GLM/glm.hpp>
#include <memory>

// Source: https://learnopengl.com/Getting-started/Camera

namespace Mimic
{
	// #############################################################################
	// camera stuct:
	// #############################################################################
	struct Camera
	{
		explicit Camera();

		glm::vec3 Position;
		glm::vec3 Right;
		glm::vec3 Up;
		glm::vec3 Direction; // needs to be reversed.
		glm::vec3 Target;
		glm::vec2 ClippingPlane;
		float Fov;
		bool MainCamera;

	private:
		friend struct MimicCore;

		glm::mat4 _projectionMatrix;
		glm::mat4 _viewMatrix;
		std::weak_ptr<Camera> Self;

		void Update();
	};
}