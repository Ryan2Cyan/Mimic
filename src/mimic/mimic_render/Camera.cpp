#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>


namespace MimicRender
{
	std::shared_ptr<Camera> Camera::Initialise(const glm::vec2& aspectRatio, const float& fov, const glm::vec2& clippingPlane)
	{
		auto camera = std::make_shared<Camera>();

		camera->ClippingPlane = clippingPlane;
		camera->AspectRatio = aspectRatio;
		camera->Fov = fov;

		camera->Position = glm::vec3(0.0f, 0.0f, 1.0f);
		camera->Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
		camera->_up = glm::vec3(0.0f, 1.0f, 0.0f);

		return camera;
	}

	void Camera::Update() 
	{
		// Calculate view and projection camera matrices:
		_viewMatrix = glm::lookAt(Position, Position + glm::normalize(Orientation), _up);
		_projectionMatrix = glm::perspective(glm::radians(Fov), AspectRatio.x / AspectRatio.y, ClippingPlane.x, ClippingPlane.y);
	}

	glm::mat4 Camera::GetProjectionMatrix() const
	{
		return _projectionMatrix;
	}

	glm::mat4 Camera::GetViewMatrix() const
	{
		return _viewMatrix;
	}
}