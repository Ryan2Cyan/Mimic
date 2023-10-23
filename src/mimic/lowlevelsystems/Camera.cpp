#include <lowlevelsystems/Mimic.h>
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Mimic
{
	Camera::Camera(const glm::vec2 aspectRatio) 
	{
		Fov = 45.0f;
		ClippingPlane = glm::vec2(0.1f, 100.0f);
		Position = glm::vec3(0.0f, 0.0f, -10.0f);
		
		// Gram-Schmidt to create LookAt matrix:
		Target = glm::vec3(0.0f);
		Direction = glm::normalize(Position - Target); 

		Up = glm::vec3(0.0f, 1.0f, 0.0f);
		Right = glm::normalize(glm::cross(Up, Direction));

		Up = glm::cross(Direction, Right);

		// create view matrix via glm::lookAt:
		_viewMatrix = glm::mat4(1.0f);
		_viewMatrix = glm::lookAt(Position, Target, Up);
		// create projection matrix:
		_projectionMatrix = glm::perspective(45.0f, aspectRatio.x / aspectRatio.y, 0.1f, 100.0f);

		/*_viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, .5f));
		_projectionMatrix = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);*/
		MainCamera = false;
	}

	void Camera::Update()
	{
		
	}

	std::shared_ptr<MimicCore> Camera::GetMimicCore() const
	{
		return _mimicCore.lock();
	}
}