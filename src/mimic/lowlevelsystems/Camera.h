#pragma once
#include <GLM/glm.hpp>
#include <memory>

// Source: https://learnopengl.com/Getting-started/Camera

namespace Mimic
{
	// #############################################################################
	// camera stuct:
	// #############################################################################
	// KARSTEN ADVICE: make this a component:
	struct MimicCore;
	struct Camera
	{
		explicit Camera(const glm::vec2 aspectRatio);
		std::shared_ptr<MimicCore> GetMimicCore() const;

		glm::vec3 Position;
		glm::vec3 Right;
		glm::vec3 Up;
		glm::vec3 Direction; // needs to be reversed.
		glm::vec3 Target;
		glm::vec2 AspectRatio;
		glm::vec2 ClippingPlane;
		float LookX;
		float Fov;
		bool MainCamera;

	private:
		friend struct MimicCore;
		friend struct ModelRenderer;

		glm::mat4 _projectionMatrix;
		glm::mat4 _viewMatrix;

		std::weak_ptr<Camera> _self;
		std::weak_ptr<MimicCore> _mimicCore;

		void Update();
	};
}