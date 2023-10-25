#include <lowlevelsystems/Mimic.h>
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Mimic
{
	Camera::Camera() 
	{
	}

	void Camera::Initialise(const glm::vec2 aspectRatio, const float fov)
	{
		Fov = fov;
		ClippingPlane = glm::vec2(0.1f, 100.0f);
		AspectRatio = glm::vec2(0.0f);
		glm::vec3 position = GetGameObject()->Position;

		// Gram-Schmidt to create LookAt matrix:
		Target = glm::vec3(0.0f);
		Direction = glm::normalize(position - Target);

		Up = glm::vec3(0.0f, 1.0f, 0.0f);
		Right = glm::normalize(glm::cross(Up, Direction));

		Up = glm::cross(Direction, Right);

		// create view matrix via glm::lookAt:
		_viewMatrix = glm::mat4(1.0f);
		_viewMatrix = glm::lookAt(position, Target, Up);
		
		// create projection matrix:
		_projectionMatrix = glm::perspective(Fov, aspectRatio.x / aspectRatio.y, ClippingPlane.x, ClippingPlane.y);
	}

	void Camera::Update()
	{

	}

	void Camera::Draw()
	{

	}
}