#include "Camera.h"
#include <utility/Logger.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Mimic
{
	void Camera::Initialise(const glm::vec2& aspectRatio, const float& fov)
	{
		Fov = fov;
		Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
		Up = glm::vec3(0.0f, 1.0f, 0.0f);
		ClippingPlane = glm::vec2(0.1f, 100.0f);
		AspectRatio = aspectRatio;
		
		_initialised = true;
		MIMIC_LOG_INFO("[Mimic::Camera] Load successful.");
	}

	void Camera::Update() 
	{
		// Initializes matrices since otherwise they will be the null matrix
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		glm::vec3 position = GetGameObject()->Position;

		// Makes camera look in the right direction from the right position
		_viewMatrix = glm::lookAt(position, position + Orientation, Up);
		// Adds perspective to the scene
		_projectionMatrix = glm::perspective(glm::radians(Fov), AspectRatio.x / AspectRatio.y, ClippingPlane.x, ClippingPlane.y);
	}
}