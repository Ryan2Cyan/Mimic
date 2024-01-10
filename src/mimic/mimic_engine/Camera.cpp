#include "Camera.h"
#include "MimicCore.h"
#include <mimic_render/Camera.h>

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

	void Camera::SetName(const std::string& name)
	{
		_name = name;
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

	std::string Camera::GetName() const
	{
		return _name;
	}

	void Camera::Update()
	{
		_renderCamera->Update();
	}
}