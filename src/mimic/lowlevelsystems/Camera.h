#pragma once
#include <lowlevelsystems/Component.h>
#include <utility/Logger.h>
#include <GLM/glm.hpp>
#include <memory>

// Source: https://learnopengl.com/Getting-started/Camera

namespace Mimic
{
	// #############################################################################
	// camera stuct:
	// #############################################################################
	struct MimicCore;
	struct Camera : Component
	{
		explicit Camera();

		void Initialise(const glm::vec2 aspectRatio, const float fov);
		glm::vec3 Right;
		glm::vec3 Up;
		glm::vec3 Direction; // needs to be reversed.
		glm::vec3 Target;
		glm::vec2 AspectRatio;
		glm::vec2 ClippingPlane;
		float Fov;

	private:
		friend struct MimicCore;
		friend struct ModelRenderer;

		glm::mat4 _projectionMatrix;
		glm::mat4 _viewMatrix;

		void Update() override;
		void Draw() override;
	};
}