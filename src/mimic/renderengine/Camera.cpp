#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace MimicRender
{
	const std::shared_ptr<Camera> Camera::Initialise(const glm::vec2& aspectRatio, const float& fov)
	{
		std::shared_ptr camera = std::make_shared<Camera>();

		camera->Position = glm::vec3(0.0f, 0.0f, 1.0f);
		camera->Fov = fov;
		camera->Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
		camera->Up = glm::vec3(0.0f, 1.0f, 0.0f);
		camera->ClippingPlane = glm::vec2(0.1f, 100.0f);
		camera->AspectRatio = aspectRatio;

		return camera;
	}

	void Camera::Update() 
	{
		// Initializes matrices since otherwise they will be the null matrix
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);

		// Makes camera look in the right direction from the right position
		_viewMatrix = glm::lookAt(Position, Position + Orientation, Up);
		// Adds perspective to the scene
		_projectionMatrix = glm::perspective(glm::radians(Fov), AspectRatio.x / AspectRatio.y, ClippingPlane.x, ClippingPlane.y);
	}
}