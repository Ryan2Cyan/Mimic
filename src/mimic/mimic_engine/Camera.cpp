#include "Camera.h"
#include "MimicCore.h"

namespace MimicEngine
{
	std::shared_ptr<Camera> Camera::Initialise(const glm::vec2& aspectRatio, const float& fov, const glm::vec2& clippingPlane)
	{
		auto camera = std::make_shared<Camera>();

		camera->_renderCamera = MimicRender::Camera::Initialise(aspectRatio, fov, clippingPlane);
		return camera;
	}

	void Camera::SetPosition(const glm::vec3& position)
	{
		_renderCamera->Position = position;
	}

	void Camera::SetOrientation(const glm::vec3& orientation)
	{
		_renderCamera->Orientation = orientation;
	}

	glm::vec3 Camera::GetPosition() const
	{
		return _renderCamera->Position;
	}

	glm::vec3 Camera::GetOrientation() const
	{
		return _renderCamera->Orientation;
	}

	glm::mat4 Camera::GetViewMatrix() const
	{
		return _renderCamera->GetViewMatrix();
	}

	glm::mat4 Camera::GetProjectionMatrix() const
	{
		return _renderCamera->GetProjectionMatrix();
	}

	void Camera::Update()
	{
		_renderCamera->Update();
	}
}